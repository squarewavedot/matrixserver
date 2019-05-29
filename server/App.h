#ifndef MATRIXSERVER_APP_H
#define MATRIXSERVER_APP_H

#include <matrixserver.pb.h>
#include <SocketConnection.h>

enum class AppState : unsigned int {
    running,
    paused,
    ended,
    killed,
    unkown
};

class App {
public:
    App(std::shared_ptr<UniversalConnection>);
    ~App() = default;

    int getAppId();

    void sendMsg(std::shared_ptr<matrixserver::MatrixServerMessage> message);

    void setAppState(AppState);

    AppState getAppState();

    std::shared_ptr<UniversalConnection> getConnection();

    int generateAppId();

private:
    int appId;
    AppState appState;
    std::shared_ptr<UniversalConnection> connection;
};


#endif //MATRIXSERVER_APP_H
