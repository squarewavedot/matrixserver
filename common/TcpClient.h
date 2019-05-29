#ifndef MATRIXSERVER_TCPCLIENT_H
#define MATRIXSERVER_TCPCLIENT_H

#include <boost/asio.hpp>
#include <functional>

#include <SocketConnection.h>

class TcpClient {
public:
//    TcpClient(boost::asio::io_service &setIo, std::string, std::string);
//
//    void handleConnect(const boost::system::error_code &error);
//
//    void doAsyncConnect();
//
//    void setConnectCallback(std::function<void(std::shared_ptr<SocketConnection>)> callback);

    static std::shared_ptr<SocketConnection> connect(boost::asio::io_service &io, std::string serverAddress, std::string serverPort);
private:
//    boost::asio::io_service &io;
//    boost::asio::ip::tcp::endpoint endpoint;
//    boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
//    std::function<void(std::shared_ptr<SocketConnection>)> connectCallback;
//    std::shared_ptr<SocketConnection> current_connection;
};


#endif //MATRIXSERVER_TCPCLIENT_H
