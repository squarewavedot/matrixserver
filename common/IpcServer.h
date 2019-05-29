#ifndef MATRIXSERVER_IPCSERVER_H
#define MATRIXSERVER_IPCSERVER_H

#include "IpcConnection.h"

class IpcServer {
public:
    IpcServer(std::string serverAddress);

    void startAccepting();

    void setAcceptCallback(std::function<void(std::shared_ptr<UniversalConnection>)> callback);

private:
    void acceptLoop();
    std::shared_ptr<boost::interprocess::message_queue> serverMQ;
    std::function<void(std::shared_ptr<UniversalConnection>)> acceptCallback;
    boost::thread *acceptThread;
    char receiveBuffer[SERVERMESSAGESIZE];
};

#endif //MATRIXSERVER_IPCSERVER_H
