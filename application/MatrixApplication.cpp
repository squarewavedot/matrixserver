#include "MatrixApplication.h"
#include <sys/time.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

MatrixApplication::MatrixApplication(int fps, std::string setServerAddress, std::string setServerPort) :
        mainThread(),
        io_context(),
        serverAddress(setServerAddress),
        serverPort(setServerPort) {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
    srand(time(NULL));
    setFps(fps);
    appId = 0;
    appState = AppState::starting;
    ioThread = new boost::thread([this]() { io_context.run(); });
    while (!connect(serverAddress, serverPort)) {
        sleep(1);
    }
    registerAtServer();
}

bool MatrixApplication::connect(const std::string &serverAddress, const std::string &serverPort) {
    BOOST_LOG_TRIVIAL(debug) << "[Application] Trying to connect to Server";

    auto ipcCon = std::make_shared<IpcConnection>();
    ipcCon->connectToServer("matrixserver");
    connection = ipcCon;
    //connection = TcpClient::connect(io_context, serverAddress, serverPort);
//    connection = UnixSocketClient::connect(io_context, "/tmp/matrixserver.sock");

    if (!connection->isDead()) {
        BOOST_LOG_TRIVIAL(debug) << "[Application] Connection successfull";
//        ioThread = new boost::thread([this]() { io_context.run(); });
        connection->setReceiveCallback(
                bind(&MatrixApplication::handleRequest, this, std::placeholders::_1, std::placeholders::_2));
        return true;
    } else {
        BOOST_LOG_TRIVIAL(debug) << "[Application] Connection failed";
        return false;
    }
}

void MatrixApplication::registerAtServer() {
    BOOST_LOG_TRIVIAL(trace) << "[Application] try to register at server";
    auto message = std::make_shared<matrixserver::MatrixServerMessage>();
    message->set_messagetype(matrixserver::registerApp);
    connection->sendMessage(message);
}

void MatrixApplication::renderToScreens() {
    auto setScreenMessage = std::make_shared<matrixserver::MatrixServerMessage>();
    setScreenMessage->set_messagetype(matrixserver::setScreenFrame);
    int i = 0;
    for (auto screen : screens) {
        auto screenData = setScreenMessage->add_screendata();
        screenData->set_screenid(screen->getScreenId());
        screenData->set_framedata((char *) screen->getScreenData().data(), screen->getScreenDataSize() * sizeof(Color));
        screenData->set_encoding(matrixserver::ScreenData_Encoding_rgb24bbp);
    }
    connection->sendMessage(setScreenMessage);
}

void MatrixApplication::internalLoop() {
    bool running = true;
    while (running) {
        long startTime = micros();
        if (appState == AppState::running) {
            running = loop();
            renderToScreens();
        }
        checkConnection();
        long sleepTime = (1000000 / fps) - (micros() - startTime);
        if (sleepTime > 0) {
            usleep(sleepTime);
        } else {
            BOOST_LOG_TRIVIAL(warning) << "[Application] FPS drop, load: " << load;
        }
        load = 1.0f - ((float) sleepTime / (1000000.0f / (float) fps));
    }
}

void MatrixApplication::checkConnection() {
    if (connection->isDead()) {
        appState = AppState::failure;
        if (connect(serverAddress, serverPort))
            registerAtServer();
    }
}

void
MatrixApplication::handleRequest(std::shared_ptr<UniversalConnection> connection,
                                 std::shared_ptr<matrixserver::MatrixServerMessage> message) {
    BOOST_LOG_TRIVIAL(trace) << "[Application] handleRequest called";
    switch (message->messagetype()) {
        case matrixserver::registerApp:
            if (message->status() == matrixserver::success) {
                BOOST_LOG_TRIVIAL(debug) << "[Application] Register at Server successfull";
                appId = message->appid();
                auto response = std::make_shared<matrixserver::MatrixServerMessage>();
                response->set_messagetype(matrixserver::getServerInfo);
                response->set_appid(appId);
                connection->sendMessage(response);
            }
            break;
        case matrixserver::getServerInfo:
            BOOST_LOG_TRIVIAL(debug) << "[Application] ServerInfo received, setup complete!";
            serverConfig.Clear();
            serverConfig.CopyFrom(message->serverconfig());
            for (auto screenInfo : serverConfig.screeninfo()) {
                screens.push_back(
                        std::make_shared<Screen>(screenInfo.width(), screenInfo.height(), screenInfo.screenid()));
            }
            appState = AppState::running;
            break;
        case matrixserver::appPause: {
            auto response = std::make_shared<matrixserver::MatrixServerMessage>();
            response->set_messagetype(matrixserver::appPause);
            response->set_appid(appId);
            if (pause())
                response->set_status(matrixserver::success);
            else
                response->set_status(matrixserver::error);
            connection->sendMessage(response);
        }
            break;
        case matrixserver::appAlive: {
            auto response = std::make_shared<matrixserver::MatrixServerMessage>();
            response->set_messagetype(matrixserver::appAlive);
            response->set_appid(appId);
            if (appState == AppState::running || appState == AppState::paused)
                response->set_status(matrixserver::success);
            else
                response->set_status(matrixserver::error);
            connection->sendMessage(response);
        }
            break;
        case matrixserver::appResume: {
            auto response = std::make_shared<matrixserver::MatrixServerMessage>();
            response->set_messagetype(matrixserver::appResume);
            response->set_appid(appId);
            if (resume())
                response->set_status(matrixserver::success);
            else
                response->set_status(matrixserver::error);
            connection->sendMessage(response);
        }
            break;
        case matrixserver::appKill: {
            auto response = std::make_shared<matrixserver::MatrixServerMessage>();
            response->set_messagetype(matrixserver::appKill);
            response->set_appid(appId);
            response->set_status(matrixserver::success);
            connection->sendMessage(response);
            stop();
        }
            break;
        case matrixserver::requestScreenAccess:
        case matrixserver::setScreenFrame:
        default:
            break;
    }
}

int MatrixApplication::getFps() {
    return fps;
}

void MatrixApplication::setFps(int setFps) {
    if (setFps <= MAXFPS && setFps >= MINFPS) {
        fps = setFps;
    } else if (setFps == 0) {
        fps = DEFAULTFPS;
    }
}

AppState MatrixApplication::getAppState() {
    return appState;
}

float MatrixApplication::getLoad() {
    return load;
}

void MatrixApplication::start() {
    mainThread = new boost::thread(&MatrixApplication::internalLoop, this);
}

bool MatrixApplication::pause() {
    if (appState == AppState::running) {
        appState = AppState::paused;
        return true;
    }
    return false;
}

bool MatrixApplication::resume() {
    if (appState == AppState::paused) {
        appState = AppState::running;
        return true;
    }
    return false;
}

void MatrixApplication::stop() {
    if (mainThread != NULL) {
        mainThread->interrupt();
        mainThread->join();
        mainThread = NULL;
    }
    appState = AppState::killed;
}

long MatrixApplication::micros() {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    long us = tp.tv_sec * 1000000 + tp.tv_usec;
    return us;
}
