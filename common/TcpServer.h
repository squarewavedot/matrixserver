#ifndef MATRIXSERVER_TCPSERVER_H
#define MATRIXSERVER_TCPSERVER_H

#include <boost/asio.hpp>
#include <functional>
#include <SocketConnection.h>

class TcpServer {
public:
    TcpServer(boost::asio::io_service &setIo, boost::asio::ip::tcp::endpoint setEndpoint);

    void handleAccept(const boost::system::error_code &error, std::shared_ptr<SocketConnection> connection);

    void doAccept();

    void setAcceptCallback(std::function<void(std::shared_ptr<SocketConnection>)> callback);

private:
    boost::asio::io_service &io;
    std::shared_ptr<SocketConnection> remote_con;
    boost::asio::ip::tcp::endpoint endpoint;
    boost::asio::ip::tcp::acceptor acceptor;
    std::function<void(std::shared_ptr<SocketConnection>)> acceptCallback;
};


#endif //MATRIXSERVER_TCPSERVER_H
