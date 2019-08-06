#include "MatrixApplicationStandalone.h"
#include <sys/time.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <thread>
#include <unistd.h>


void createDefaultCubeConfig(matrixserver::ServerConfig &serverConfig) {
    serverConfig.Clear();
    serverConfig.set_globalscreenbrightness(100);
    serverConfig.set_servername("matrixserver");
    matrixserver::Connection *serverConnection = new matrixserver::Connection();
    serverConnection->set_serveraddress("127.0.0.1");
    serverConnection->set_serverport("2017");
    serverConnection->set_connectiontype(matrixserver::Connection_ConnectionType_tcp);
    serverConfig.set_allocated_serverconnection(serverConnection);
    serverConfig.set_assemblytype(matrixserver::ServerConfig_AssemblyType_cube);
    for (int i = 0; i < 6; i++) {
        auto screenInfo = serverConfig.add_screeninfo();
        screenInfo->set_screenid(i);
        screenInfo->set_available(true);
        screenInfo->set_height(64);
        screenInfo->set_width(64);
        screenInfo->set_screenorientation((matrixserver::ScreenInfo_ScreenOrientation) (i + 1));
    }
}


MatrixApplicationStandalone::MatrixApplicationStandalone(int fps, std::string setServerAddress, std::string setServerPort) :
        mainThread(), renderThread() {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
    newFrame = false;

    createDefaultCubeConfig(serverConfig);

    BOOST_LOG_TRIVIAL(info) << "ServerConfig: " << std::endl << serverConfig.DebugString() << std::endl;

    for (auto screenInfo : serverConfig.screeninfo()){
        auto screen = std::make_shared<Screen>(screenInfo.width(), screenInfo.height(), screenInfo.screenid());
        switch(screenInfo.screenorientation()){
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_front :
                screen->setOffsetX(1);
                screen->setOffsetY(1);
                screen->setRotation(Rotation::rot180);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_right :
                screen->setOffsetX(0);
                screen->setOffsetY(1);
                screen->setRotation(Rotation::rot180);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_back :
                screen->setOffsetX(1);
                screen->setOffsetY(0);
                screen->setRotation(Rotation::rot90);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_left :
                screen->setOffsetX(2);
                screen->setOffsetY(1);
                screen->setRotation(Rotation::rot180);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_top :
                screen->setOffsetX(0);
                screen->setOffsetY(0);
                screen->setRotation(Rotation::rot270);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_bottom :
                screen->setOffsetX(2);
                screen->setOffsetY(0);
                screen->setRotation(Rotation::rot270);
                break;
            default:
                break;
        }
        renderscreens.push_back(screen);
    }

    for (auto screenInfo : serverConfig.screeninfo()){
        auto screen = std::make_shared<Screen>(screenInfo.width(), screenInfo.height(), screenInfo.screenid());
        switch(screenInfo.screenorientation()){
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_front :
                screen->setOffsetX(1);
                screen->setOffsetY(1);
                screen->setRotation(Rotation::rot180);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_right :
                screen->setOffsetX(0);
                screen->setOffsetY(1);
                screen->setRotation(Rotation::rot180);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_back :
                screen->setOffsetX(1);
                screen->setOffsetY(0);
                screen->setRotation(Rotation::rot90);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_left :
                screen->setOffsetX(2);
                screen->setOffsetY(1);
                screen->setRotation(Rotation::rot180);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_top :
                screen->setOffsetX(0);
                screen->setOffsetY(0);
                screen->setRotation(Rotation::rot270);
                break;
            case matrixserver::ScreenInfo_ScreenOrientation::ScreenInfo_ScreenOrientation_bottom :
                screen->setOffsetX(2);
                screen->setOffsetY(0);
                screen->setRotation(Rotation::rot270);
                break;
            default:
                break;
        }
        screens.push_back(screen);
    }

    renderer = std::make_shared<FPGARendererRPISPI>(renderscreens);

    appState = AppState::starting;
}

void MatrixApplicationStandalone::renderToScreens() {
//    renderSyncMutex.lock();
//    renderSyncMutex.unlock();
    renderer->render();
//    std::thread([this](){renderer->render();}).detach();
}

void MatrixApplicationStandalone::renderLoop() {
    bool running = true;
    while (running) {
//        renderSyncMutex.lock();
        while(newFrame == false)
            usleep(100);
        for (auto screen : screens) {
            renderer->setScreenData(screen->getScreenId(), screen->getScreenDataRaw());
        }
        newFrame = false;
//        renderSyncMutex.unlock();
//        auto startTime = micros();
        renderToScreens();
//      BOOST_LOG_TRIVIAL(warning) << "[Application] rendertime: " << micros()-startTime << " us";
    }
}

void MatrixApplicationStandalone::internalLoop() {
    bool running = true;
    while (running) {
        auto startTime = micros();
        if (appState == AppState::running) {
            while(newFrame == true)
                usleep(100);
            running = loop();
            newFrame = true;
        }
        BOOST_LOG_TRIVIAL(warning) << "[Application] rendertime: " << micros()-startTime << " us";
    }
}


int MatrixApplicationStandalone::getFps() {
    return fps;
}

void MatrixApplicationStandalone::setFps(int setFps) {
    if (setFps <= MAXFPS && setFps >= MINFPS) {
        fps = setFps;
    } else if (setFps == 0) {
        fps = DEFAULTFPS;
    }
}

AppState MatrixApplicationStandalone::getAppState() {
    return appState;
}

float MatrixApplicationStandalone::getLoad() {
    return load;
}

void MatrixApplicationStandalone::start() {
    mainThread = new boost::thread(&MatrixApplicationStandalone::internalLoop, this);
    renderThread = new boost::thread(&MatrixApplicationStandalone::renderLoop, this);
    appState = AppState::running;
}

bool MatrixApplicationStandalone::pause() {
    if (appState == AppState::running) {
        appState = AppState::paused;
        return true;
    }
    return false;
}

bool MatrixApplicationStandalone::resume() {
    if (appState == AppState::paused) {
        appState = AppState::running;
        return true;
    }
    return false;
}

void MatrixApplicationStandalone::stop() {
    if (mainThread != NULL) {
        mainThread->interrupt();
        mainThread->join();
        mainThread = NULL;
    }
    appState = AppState::killed;
}

long MatrixApplicationStandalone::micros() {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    long us = tp.tv_sec * 1000000 + tp.tv_usec;
    return us;
}
