#ifndef MATRIXSERVER_UNIXSOCKETSERVER_H
#define MATRIXSERVER_UNIXSOCKETSERVER_H

#include <boost/asio.hpp>
#include <functional>
#include <SocketConnection.h>

class UnixSocketServer {
public:
    UnixSocketServer(boost::asio::io_service &setIo, boost::asio::local::stream_protocol::endpoint setEndpoint);

    void handleAccept(const boost::system::error_code &error, std::shared_ptr<SocketConnection> connection);

    void doAccept();

    void setAcceptCallback(std::function<void(std::shared_ptr<SocketConnection>)> callback);

private:
    boost::asio::io_service &io;
    std::shared_ptr<SocketConnection> remote_con;
    boost::asio::local::stream_protocol::endpoint endpoint;
    boost::asio::local::stream_protocol::acceptor acceptor;
    std::function<void(std::shared_ptr<SocketConnection>)> acceptCallback;
};


#endif //MATRIXSERVER_UNIXSOCKETSERVER_H
