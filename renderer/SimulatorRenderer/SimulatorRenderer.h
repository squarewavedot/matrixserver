#ifndef MATRIXSERVER_SIMULATORRENDERER_H
#define MATRIXSERVER_SIMULATORRENDERER_H

#include <IRenderer.h>
#include <TcpClient.h>
#include <boost/thread/thread.hpp>

#define DEFAULTSERVERADRESS "127.0.0.1"
#define DEFAULTSERVERPORT "1337"


class SimulatorRenderer : public IRenderer {
public:
    SimulatorRenderer();

    SimulatorRenderer(std::vector<std::shared_ptr<Screen>> screens, std::string setServerAddress = DEFAULTSERVERADRESS,
                      std::string setServerPort = DEFAULTSERVERPORT);

    void init(std::vector<std::shared_ptr<Screen>>);

    void setScreenData(int, Color *);

    void render();

    void setGlobalBrightness(int);

    int getGlobalBrightness();

private:
    bool connect();

    std::string serverAddress;
    std::string serverPort;
    std::shared_ptr<SocketConnection> connection;
    boost::thread *mainThread;
    boost::thread *ioThread;
    boost::asio::io_service io_context;
};


#endif //MATRIXSERVER_SIMULATORRENDERER_H
