#include "IpcServer.h"
#include <boost/log/trivial.hpp>



IpcServer::IpcServer(std::string serverAddress)
{
    boost::interprocess::message_queue::remove(serverAddress.data());
    serverMQ = std::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_or_create, serverAddress.data(), 10, SERVERMESSAGESIZE, boost::interprocess::permissions(0666));
    acceptCallback = NULL;
    startAccepting();
    BOOST_LOG_TRIVIAL(debug) << "[Server] Start accepting on IPC Channel: " << serverAddress;
}

void IpcServer::startAccepting() {
    acceptThread = new boost::thread(&IpcServer::acceptLoop, this);
}

void IpcServer::acceptLoop() {
    while(1){
        boost::interprocess::message_queue::size_type recvd_size;
        unsigned int priority;
        this->serverMQ->receive(&receiveBuffer, SERVERMESSAGESIZE, recvd_size, priority); //blocking

        std::stringstream sendMQname;
        for(int i = 0; i < 20; i++)
            sendMQname << (char)(rand()%26+'a'); // add random character [a...z]

        auto receiveMQ = std::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_or_create, sendMQname.str().data(), 10, SERVERMESSAGESIZE, boost::interprocess::permissions(0666));
        auto sendMQ = std::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, std::string(receiveBuffer, recvd_size).data());
        sendMQ->send(sendMQname.str().data(), sendMQname.str().size(), 0);

        BOOST_LOG_TRIVIAL(debug) << "[Server] Accepted Connection, sendMQ " << receiveBuffer << " receiveMQ " << sendMQname.str();

        auto connection = std::make_shared<IpcConnection>(sendMQ, receiveMQ);
        if (acceptCallback != NULL) {
            connection->startReceiving();
            acceptCallback(connection);
        }
    }
}

void IpcServer::setAcceptCallback(std::function<void(std::shared_ptr<UniversalConnection>)> callback) {
    acceptCallback = callback;
}
