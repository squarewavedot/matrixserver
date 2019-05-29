#ifndef MATRIXSERVER_SOCKETCONNECTION_H
#define MATRIXSERVER_SOCKETCONNECTION_H

#include <boost/asio.hpp>
#include <functional>
#include <string>
#include <mutex>
#include "Cobs.h"
#include <matrixserver.pb.h>
#include "UniversalConnection.h"

#define RECEIVE_BUFFER_SIZE 200000

class SocketConnection :  public std::enable_shared_from_this<SocketConnection>, public UniversalConnection {
public:
    SocketConnection(boost::asio::io_service &io_context);

    ~SocketConnection();

    boost::asio::generic::stream_protocol::socket &getSocket();

    void startReceiving();

    void
    setReceiveCallback(std::function<void(std::shared_ptr<UniversalConnection>,
                                          std::shared_ptr<matrixserver::MatrixServerMessage>)> callback);

    void sendMessage(std::shared_ptr<matrixserver::MatrixServerMessage> message);

    bool isDead();

    void setDead(bool sDead);

private:
    void doRead();

    void handleWrite(const boost::system::error_code &error, size_t bytes_transferred,
                     const std::string & message_encoded);

    void handleRead(const boost::system::error_code &error, size_t bytes_transferred);

    boost::asio::io_service &io;
    boost::asio::generic::stream_protocol::socket socket;
    std::mutex sendMutex;
    char recv_buffer[RECEIVE_BUFFER_SIZE];
    std::string message_buffer;
    Cobs cobsDecoder;
    std::function<void(std::shared_ptr<UniversalConnection>,
                       std::shared_ptr<matrixserver::MatrixServerMessage>)> receiveCallback;
    bool dead = false;
};


#endif //MATRIXSERVER_SOCKETCONNECTION_H
