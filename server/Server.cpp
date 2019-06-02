#include <vector>
#include <iostream>
#include <future>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <sys/time.h>

#include "Server.h"

#include <chrono>

App * Server::getAppByID(int searchID){
    for(auto app : apps){
        if(app.getAppId() == searchID)
            return & app;
    }
    return nullptr;
}

Server::Server(std::shared_ptr<IRenderer> setRenderer, matrixserver::ServerConfig & setServerConfig) :
        ioContext(),
        serverConfig(setServerConfig),
        tcpServer(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(setServerConfig.serverconnection().serverport()))),
        unixServer(ioContext, boost::asio::local::stream_protocol::endpoint("/tmp/matrixserver.sock")),
        ipcServer("matrixserver"){
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
    renderers.push_back(setRenderer);
    tcpServer.setAcceptCallback(std::bind(&Server::newConnectionCallback, this, std::placeholders::_1));
    unixServer.setAcceptCallback(std::bind(&Server::newConnectionCallback, this, std::placeholders::_1));
    ipcServer.setAcceptCallback(std::bind(&Server::newConnectionCallback, this, std::placeholders::_1));
    ioThread = new boost::thread([this]() { this->ioContext.run(); });
    srand(time(NULL));
}

void Server::newConnectionCallback(std::shared_ptr<UniversalConnection> connection) {
    BOOST_LOG_TRIVIAL(debug) << "[matrixserver] NEW SocketConnection CALLBACK!";
    connection->setReceiveCallback(
            std::bind(&Server::handleRequest, this, std::placeholders::_1, std::placeholders::_2));
    connections.push_back(connection);
}

void Server::handleRequest(std::shared_ptr<UniversalConnection> connection, std::shared_ptr<matrixserver::MatrixServerMessage> message) {
    switch (message->messagetype()) {
        case matrixserver::registerApp:
            if (message->appid() == 0) {
                BOOST_LOG_TRIVIAL(debug) << "[matrixserver] register new App request received";
                apps.push_back(App(connection));
                auto response = std::make_shared<matrixserver::MatrixServerMessage>();
                response->set_appid(apps.back().getAppId());
                response->set_messagetype(matrixserver::registerApp);
                response->set_status(matrixserver::success);
                connection->sendMessage(response);
            }
            break;
        case matrixserver::getServerInfo: {
            BOOST_LOG_TRIVIAL(debug) << "[matrixserver] get ServerInfo request received";
            auto response = std::make_shared<matrixserver::MatrixServerMessage>();
            response->set_messagetype(matrixserver::getServerInfo);
            auto * tempServerConfig = new matrixserver::ServerConfig();
            tempServerConfig->CopyFrom(serverConfig);
            response->set_allocated_serverconfig(tempServerConfig);
            connection->sendMessage(response);
            break;
        }
        case matrixserver::requestScreenAccess:
            //TODO App level logic, set App on top, pause all other Apps, which aren't on top any more
            break;
        case matrixserver::setScreenFrame:
            //TODO check if message.->appID is current top application to write screen data
//            BOOST_LOG_TRIVIAL(debug) << "[matrixserver] new ScreenFrame received";
            if(message->appid() == apps.back().getAppId()){
                for(auto renderer : renderers){
                    for(auto screenInfo : message->screendata()){
                        renderer->setScreenData(screenInfo.screenid(), (Color *)screenInfo.framedata().data()); //TODO: remove C style cast
                    }

                    auto usStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
//                    std::thread([renderer](){renderer->render();}).detach();
                renderer->render();
                    auto usTotal = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
//                std::cout << usTotal.count() << " us" << std::endl; // ~ 15ms
                }
            }else{
                //send app to pause
                std::cout << "send app " << message->appid() << " to pause" << std::endl;
                auto msg = std::make_shared<matrixserver::MatrixServerMessage>();
                msg->set_messagetype(matrixserver::appPause);
                connection->sendMessage(msg);
            }
            break;
        case matrixserver::appAlive:
        case matrixserver::appPause:
        case matrixserver::appResume:
        case matrixserver::appKill:
        default:
            break;
    }
}

bool Server::tick() {
    apps.erase(std::remove_if(apps.begin(), apps.end(), [](App a) {
        bool returnVal = a.getConnection()->isDead();
        if(returnVal)
            BOOST_LOG_TRIVIAL(debug) << "[matrixserver] App " << a.getAppId() << " deleted";
        return returnVal;
    }), apps.end());

    connections.erase(std::remove_if(connections.begin(), connections.end(), [](std::shared_ptr <UniversalConnection> con) {
        bool returnVal = con->isDead();
        if(returnVal){
            BOOST_LOG_TRIVIAL(debug) << "[matrixserver] Connection deleted";
        }
        return returnVal;
    }), connections.end());
    return true;
}

void Server::addRenderer(std::shared_ptr<IRenderer> newRenderer) {
    renderers.push_back(newRenderer);
}
