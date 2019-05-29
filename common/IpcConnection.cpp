#include "IpcConnection.h"

#include <boost/log/trivial.hpp>

IpcConnection::IpcConnection(){
    receiveCallback = NULL;
}

IpcConnection::IpcConnection(std::shared_ptr<boost::interprocess::message_queue> sender, std::shared_ptr<boost::interprocess::message_queue> receiver){
    sendMQ = sender;
    receiveMQ = receiver;
    receiveCallback = NULL;
}


void IpcConnection::startReceiving() {
    receiveThread = new boost::thread(&IpcConnection::readLoop, this);
}

void IpcConnection::setReceiveCallback(
        std::function<void(std::shared_ptr<UniversalConnection>,
                           std::shared_ptr<matrixserver::MatrixServerMessage>)> callback) {
    receiveCallback = callback;
}

void IpcConnection::readLoop() {
    boost::interprocess::message_queue::size_type recvd_size;
    unsigned int priority;
    BOOST_LOG_TRIVIAL(trace) << "[IpcConnection] start read loop";
    while(1){
        this->receiveMQ->receive(&receiveData, MAXIPCMESSAGESIZE, recvd_size, priority); //blocking
        auto receiveMessage = std::make_shared<matrixserver::MatrixServerMessage>();
        if (receiveMessage->ParseFromString(std::string(receiveData, recvd_size))) {
            BOOST_LOG_TRIVIAL(trace) << "[IpcConnection] Recieved full Protobuf MatrixServerMessage";
            if (this->receiveCallback != NULL) {
                this->receiveCallback(shared_from_this(), receiveMessage);
            }
        }
    }
}

void IpcConnection::doRead() {
//    std::thread([this](){
//        char tempData[MAXIPCMESSAGESIZE];
//        boost::interprocess::message_queue::size_type recvd_size;
//        unsigned int priority;
//        this->receiveMQ->receive(&tempData, MAXIPCMESSAGESIZE, recvd_size, priority); //blocking
//        BOOST_LOG_TRIVIAL(debug) << "[IpcConnection] Recieved something";
//        auto receiveMessage = std::make_shared<matrixserver::MatrixServerMessage>();
//        if (receiveMessage->ParseFromString(std::string(tempData, recvd_size))) {
//            BOOST_LOG_TRIVIAL(trace) << "[IpcConnection] Recieved full Protobuf MatrixServerMessage";
//            if (this->receiveCallback != NULL) {
//                this->receiveCallback(shared_from_this(), receiveMessage);
//            }
//        }
//        this->doRead();
//    }).detach();
}


void IpcConnection::sendMessage(std::shared_ptr<matrixserver::MatrixServerMessage> message) {
    auto sendBuffer = message->SerializeAsString();
    sendMQ->send(sendBuffer.data(), sendBuffer.size(), 0);
}

bool IpcConnection::isDead() {
    return dead;
}

IpcConnection::~IpcConnection() {

}

void IpcConnection::setDead(bool sDead) {
    dead = sDead;
}

bool IpcConnection::connectToServer(std::string serverAddress) {
    try {
        std::stringstream receiveMQname;
        for(int i = 0; i < 20; i++)
            receiveMQname << (char)(rand()%26+'a'); // add random character [a...z]

        auto tempServer = std::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, serverAddress.data());
        this->receiveMQ = std::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_or_create, receiveMQname.str().data(), 1, MAXIPCMESSAGESIZE, boost::interprocess::permissions(0666));
        tempServer->send(receiveMQname.str().data(), receiveMQname.str().size(), 0);
        char tempData[MAXIPCMESSAGESIZE];
        boost::interprocess::message_queue::size_type recvd_size;
        unsigned int priority;
        this->receiveMQ->receive(&tempData, MAXIPCMESSAGESIZE, recvd_size, priority); //blocking
        if(recvd_size == 20){
            this->sendMQ = std::make_shared<boost::interprocess::message_queue>(boost::interprocess::open_only, std::string(tempData, recvd_size).data());
            this->startReceiving();
            setDead(false);
        }else{
            setDead(true);
        }
    } catch (boost::interprocess::interprocess_exception e) {
        BOOST_LOG_TRIVIAL(debug) << "[IpcConnection] " << e.what();
        setDead(true);
        return false;
    }
    return true;
}

