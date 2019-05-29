#ifndef MATRIXSERVER_IPCCONNECTION_H
#define MATRIXSERVER_IPCCONNECTION_H

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread/thread.hpp>
#include <functional>
#include <string>
#include <mutex>
#include <matrixserver.pb.h>
#include "UniversalConnection.h"

#define SERVERMESSAGESIZE 1000000
#define MAXIPCMESSAGESIZE 1000000

class IpcConnection :  public std::enable_shared_from_this<IpcConnection>, public UniversalConnection {
public:
    IpcConnection();

    IpcConnection(std::shared_ptr<boost::interprocess::message_queue> sender, std::shared_ptr<boost::interprocess::message_queue> receiver);

    ~IpcConnection();

    bool connectToServer(std::string serverAddress);

    void startReceiving();

    void
    setReceiveCallback(std::function<void(std::shared_ptr<UniversalConnection>,
                                          std::shared_ptr<matrixserver::MatrixServerMessage>)> callback);

    void sendMessage(std::shared_ptr<matrixserver::MatrixServerMessage> message);

    bool isDead();

    void setDead(bool sDead);
private:
    void doRead();

    void readLoop();

    std::shared_ptr<boost::interprocess::message_queue> sendMQ;
    std::shared_ptr<boost::interprocess::message_queue> receiveMQ;

    boost::thread * receiveThread;

    std::mutex sendMutex;
    std::string message_buffer;
    std::function<void(std::shared_ptr<UniversalConnection>,
                       std::shared_ptr<matrixserver::MatrixServerMessage>)> receiveCallback;
    bool dead = false;

    char receiveData[MAXIPCMESSAGESIZE];
};


#endif //MATRIXSERVER_IPCCONNECTION_H
