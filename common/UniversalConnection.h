#ifndef MATRIXSERVER_UNIVERSALCONNECTION_H
#define MATRIXSERVER_UNIVERSALCONNECTION_H

#include <boost/asio.hpp>
#include <functional>
#include <string>
#include <mutex>
#include <matrixserver.pb.h>

class UniversalConnection{
public:
    virtual void startReceiving() = 0;

    virtual void setReceiveCallback(std::function<void(std::shared_ptr<UniversalConnection>,
                                          std::shared_ptr<matrixserver::MatrixServerMessage>)> callback) = 0;

    virtual void sendMessage(std::shared_ptr<matrixserver::MatrixServerMessage> message) = 0;

    virtual bool isDead() = 0;

    virtual void setDead(bool sDead) = 0;
};

#endif //MATRIXSERVER_UNIVERSALCONNECTION_H
