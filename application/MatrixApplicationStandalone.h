#ifndef MATRIXSERVER_MATRIXAPPLICATIONSTANDALONE_H
#define MATRIXSERVER_MATRIXAPPLICATIONSTANDALONE_H

#include <boost/thread/thread.hpp>

#include <Screen.h>
#include <TcpClient.h>
#include <UnixSocketClient.h>
#include <IpcConnection.h>
#include <mutex>
#include <atomic>

#include <matrixserver.pb.h>
#include <google/protobuf/util/json_util.h>

#include <FPGARenderer/FPGARendererRPISPI.h>

#define DEFAULTFPS 40
#define MAXFPS 200
#define MINFPS 1

#define DEFAULTSERVERADRESS "127.0.0.1"
#define DEFAULTSERVERPORT "2017"

enum class AppState {
    starting, running, paused, ended, killed, failure
};

class MatrixApplicationStandalone {
public:
    MatrixApplicationStandalone(
            int fps = DEFAULTFPS,
            std::string setServerAddress = DEFAULTSERVERADRESS,
            std::string setServerPort = DEFAULTSERVERPORT);

    ~MatrixApplicationStandalone() = default;

    void renderToScreens();

    int getFps();

    void setFps(int fps);

    AppState getAppState();

    float getLoad();

    void start();

    bool pause();

    bool resume();

    void stop();

    virtual bool loop() = 0;

protected:
    std::vector<std::shared_ptr<Screen>> screens;
    std::vector<std::shared_ptr<Screen>> renderscreens;
    long micros();

private:
    void internalLoop();
    void renderLoop();

    int appId;
    int fps;
    float load;
    boost::thread *mainThread;
    boost::thread *renderThread;
    AppState appState;

    std::mutex renderSyncMutex;

    std::atomic<bool> newFrame;

    matrixserver::ServerConfig serverConfig;

    std::shared_ptr<IRenderer> renderer;
};


#endif //MATRIXSERVER_MATRIXAPPLICATIONSTANDALONE_H