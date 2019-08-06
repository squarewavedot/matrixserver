#include <vector>
#include <iostream>
#include <future>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <sys/time.h>
#include <random>
#include <chrono>

#include "Server.h"

std::string defaultApp("/home/pi/matrixserver/cmake-build-release-raspberrypi/exampleApplications/MainMenu/MainMenu 1>/dev/null 2>/dev/null &");
bool defaultAppStarted = false;

App * Server::getAppByID(int searchID){
    for(unsigned int i = 0; i < apps.size(); i++){
        if(apps[i].getAppId() == searchID){
            return & apps[i];
        }
    }
    return nullptr;
}

Server::Server(std::shared_ptr<IRenderer> setRenderer, matrixserver::ServerConfig & setServerConfig) :
        ioContext(),
        serverConfig(setServerConfig),
        tcpServer(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(setServerConfig.serverconnection().serverport()))),
//        unixServer(ioContext, boost::asio::local::stream_protocol::endpoint("/tmp/matrixserver.sock")),
        ipcServer("matrixserver"){
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
    renderers.push_back(setRenderer);
    tcpServer.setAcceptCallback(std::bind(&Server::newConnectionCallback, this, std::placeholders::_1));
//    unixServer.setAcceptCallback(std::bind(&Server::newConnectionCallback, this, std::placeholders::_1));
    ipcServer.setAcceptCallback(std::bind(&Server::newConnectionCallback, this, std::placeholders::_1));
    ioThread = new boost::thread([this]() { this->ioContext.run(); });
    std::random_device rd;
    srand(rd());

    joysticks.push_back(new Joystick(0));
    joysticks.push_back(new Joystick(1));
    joysticks.push_back(new Joystick(2));
    joysticks.push_back(new Joystick(3));
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
            if(message->appid() == apps.back().getAppId()){
                for(auto renderer : renderers){
                    for(auto screenInfo : message->screendata()){
                        renderer->setScreenData(screenInfo.screenid(), (Color *)screenInfo.framedata().data()); //TODO: remove C style cast
                    }
                    auto usStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
    //              std::thread([renderer](){renderer->render();}).detach();
                    renderer->render();
                    auto response = std::make_shared<matrixserver::MatrixServerMessage>();
                    response->set_messagetype(matrixserver::setScreenFrame);
                    response->set_status(matrixserver::success);
                    connection->sendMessage(response);
                    auto usTotal = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()) - usStart;
                    BOOST_LOG_TRIVIAL(trace) << "[Server] rendertime: " << usTotal.count() << " us"; // ~ 15ms
                }
            }else{
                //send app to pause
                BOOST_LOG_TRIVIAL(debug) << "[Server] send app " << message->appid() << " to pause";
                auto msg = std::make_shared<matrixserver::MatrixServerMessage>();
                msg->set_messagetype(matrixserver::appKill);
                connection->sendMessage(msg);
            }
            break;
        case matrixserver::appAlive:
        case matrixserver::appPause:
        case matrixserver::appResume:
        case matrixserver::appKill:
            BOOST_LOG_TRIVIAL(debug) << "[Server] appkill " << message->appid() << " successfull";
            apps.erase(std::remove_if(apps.begin(), apps.end(), [message](App a) {
                if(a.getAppId() == message->appid()){
                    BOOST_LOG_TRIVIAL(debug) << "[Server] App " << message->appid() << " deleted";
                    return true;
                }else{
                    return false;
                }
            }), apps.end());
        default:
            break;
    }
}

bool Server::tick() {
    for (auto joystick : joysticks) {
        if (joystick->getButtonPress(11)) {
            if (apps.size() > 0) {
                BOOST_LOG_TRIVIAL(debug) << "kill current app" << std::endl;
                auto msg = std::make_shared<matrixserver::MatrixServerMessage>();
                msg->set_messagetype(matrixserver::appKill);
                apps.back().sendMsg(msg);
            }
        }
        joystick->clearAllButtonPresses();
    }

    if (apps.size() == 0 && !defaultAppStarted) {
        BOOST_LOG_TRIVIAL(debug) << "starting default app" << std::endl;
        system(defaultApp.data());
        defaultAppStarted = true;
    }
    if (apps.size() > 0) {
        defaultAppStarted = false;
    }

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
