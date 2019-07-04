#include "TcpServer.h"
#include <boost/log/trivial.hpp>

TcpServer::TcpServer(boost::asio::io_service &setIo, boost::asio::ip::tcp::endpoint setEndpoint) :
        io(setIo),
        acceptor(setIo, setEndpoint),
        endpoint(setEndpoint) {
    this->doAccept();
    acceptCallback = NULL;
}

void TcpServer::doAccept() {
    BOOST_LOG_TRIVIAL(debug) << "[Server] Start accepting on address: " << this->endpoint.address().to_string()
                             << " and port: " << this->endpoint.port();
    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    auto tempCon = std::make_shared<SocketConnection>(io);
    this->acceptor.async_accept(tempCon->getSocket(),
                                [this, tempCon](boost::system::error_code error) {
                                    this->handleAccept(error, tempCon);
                                });
}

void TcpServer::handleAccept(const boost::system::error_code &error, std::shared_ptr<SocketConnection> connection) {
    if (!error) {
        BOOST_LOG_TRIVIAL(debug) << "[Server] Accepted Connection";
        connection->startReceiving();
        if (acceptCallback != NULL) {
            acceptCallback(connection);
        }
        doAccept();
    } else {
        BOOST_LOG_TRIVIAL(debug) << "[Server] Server Accept Error: " << error.message();
    }
}

void TcpServer::setAcceptCallback(std::function<void(std::shared_ptr<SocketConnection>)> callback) {
    acceptCallback = callback;
}
