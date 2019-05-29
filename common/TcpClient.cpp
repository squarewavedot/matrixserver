#include "TcpClient.h"
#include <boost/log/trivial.hpp>
//
//TcpClient::TcpClient(boost::asio::io_service &setIo, std::string serverAddress, std::string serverPort) :
//        io(setIo),
//        endpoint_iterator(boost::asio::ip::tcp::resolver(io).resolve(boost::asio::ip::tcp::resolver::query(serverAddress, serverPort))) {
//    this->endpoint = *endpoint_iterator;
//    connectCallback = nullptr;
//}
//
//
//void TcpClient::doAsyncConnect() {
//    BOOST_LOG_TRIVIAL(debug) << "[TcpClient] connecting to address: " << this->endpoint.address().to_string()
//                             << " and port: " << this->endpoint.port();
//    this->current_connection = std::make_shared<SocketConnection>(io);
//    this->current_connection->getSocket().async_connect(this->endpoint,
//                                                        [this](boost::system::error_code error) {
//                                                            this->handleConnect(error);
//                                                        });
////    boost::asio::async_connect(this->current_connection->getSocket(), this->endpoint_iterator,
////                               [this](boost::system::error_code error,
////                                      boost::asio::ip::tcp::resolver::iterator iterator) {
////                                   this->handleConnect(error);
////                               });
//}
//
//void TcpClient::handleConnect(const boost::system::error_code &error) {
//    if (!error) {
//        BOOST_LOG_TRIVIAL(debug) << "[TcpClient] Connect Successfull to address: "
//                                 << this->endpoint.address().to_string()
//                                 << " and port: " << this->endpoint.port();
//        if (connectCallback != nullptr) {
//            BOOST_LOG_TRIVIAL(trace) << "[TcpClient] calling connect callback now";
//            connectCallback(current_connection);
//            this->current_connection->startReceiving();
//        } else {
//            BOOST_LOG_TRIVIAL(trace) << "[TcpClient] no connect callback defined";
//        }
//    } else {
//        BOOST_LOG_TRIVIAL(debug) << "[TcpClient] Connect Error: " << error.message() << " Address: "
//                                 << this->endpoint.address().to_string()
//                                 << " and port: " << this->endpoint.port();
//    }
//}
//
//
//void TcpClient::setConnectCallback(std::function<void(std::shared_ptr<SocketConnection>)> callback) {
//    connectCallback = callback;
//    this->doAsyncConnect();
//}

std::shared_ptr<SocketConnection>
TcpClient::connect(boost::asio::io_service &io, std::string serverAddress, std::string serverPort) {
    auto sockConnection = std::make_shared<SocketConnection>(io);
    try {
        boost::asio::ip::tcp::resolver::iterator endpoints = boost::asio::ip::tcp::resolver(io).resolve(boost::asio::ip::tcp::resolver::query(serverAddress, serverPort));
        boost::asio::ip::tcp::endpoint endpoint = *endpoints;
        sockConnection->getSocket().connect(endpoint);
        if (sockConnection->getSocket().is_open()) {
            BOOST_LOG_TRIVIAL(debug) << "[TcpClient] Connect Successful to address: "
                                     << endpoint.address().to_string()
                                     << " and port: " << endpoint.port();
            sockConnection->startReceiving();
        }
    } catch (boost::system::system_error e) {
        BOOST_LOG_TRIVIAL(debug) << "[TcpClient] " << e.what();
        sockConnection->setDead(true);
    }
    return sockConnection;
}
