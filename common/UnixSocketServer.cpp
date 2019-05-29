#include "UnixSocketServer.h"
#include <boost/log/trivial.hpp>

UnixSocketServer::UnixSocketServer(boost::asio::io_service &setIo, boost::asio::local::stream_protocol::endpoint setEndpoint) :
        io(setIo),
        endpoint(setEndpoint),
        acceptor(setIo){
    ::unlink(endpoint.path().data());
    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    acceptor.bind(endpoint);
    acceptor.listen();
    this->doAccept();
    acceptCallback = NULL;
}

void UnixSocketServer::doAccept() {
    BOOST_LOG_TRIVIAL(debug) << "[Server] Start accepting on path: " << this->endpoint.path();
    auto tempCon = std::make_shared<SocketConnection>(io);
    this->acceptor.async_accept(tempCon->getSocket(),
                                [this, tempCon](boost::system::error_code error) {
                                    this->handleAccept(error, tempCon);
                                });
}

void UnixSocketServer::handleAccept(const boost::system::error_code &error, std::shared_ptr<SocketConnection> connection) {
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

void UnixSocketServer::setAcceptCallback(std::function<void(std::shared_ptr<SocketConnection>)> callback) {
    acceptCallback = callback;
}
