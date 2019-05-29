#ifndef MATRIXSERVER_UNIXSOCKETCLIENT_H
#define MATRIXSERVER_UNIXSOCKETCLIENT_H

#include <SocketConnection.h>

class UnixSocketClient {
public:
    static std::shared_ptr<SocketConnection> connect(boost::asio::io_service &io, std::string socketFile);
};


#endif //MATRIXSERVER_UNIXSOCKETCLIENT_H
