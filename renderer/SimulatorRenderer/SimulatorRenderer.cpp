#include "SimulatorRenderer.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

SimulatorRenderer::SimulatorRenderer() : mainThread(),
                                         io_context(),
                                         serverAddress(DEFAULTSERVERADRESS),
                                         serverPort(DEFAULTSERVERPORT){

}

SimulatorRenderer::SimulatorRenderer(std::vector<std::shared_ptr<Screen>> screens, std::string setServerAddress,
        std::string setServerPort) : mainThread(),
                                                                                         io_context(),
                                                                                         serverAddress(setServerAddress),
                                                                                         serverPort(setServerPort) {
    init(screens);
    while (!connect()) {
        sleep(1);
    }
}

bool SimulatorRenderer::connect(){
    BOOST_LOG_TRIVIAL(debug) << "[Renderer] Trying to connect to Server";
    connection = TcpClient::connect(io_context, serverAddress, serverPort);
    if (!connection->isDead()) {
        BOOST_LOG_TRIVIAL(debug) << "[Renderer] Connection successful";
        ioThread = new boost::thread([this]() { io_context.run(); });
//        connection->setReceiveCallback(bind(&MatrixApplication::handleRequest, this, std::placeholders::_1, std::placeholders::_2));
        return true;
    } else {
        BOOST_LOG_TRIVIAL(debug) << "[Renderer] Connection failed";
        return false;
    }
}

void SimulatorRenderer::init(std::vector<std::shared_ptr<Screen>> initScreens) {
    screens = initScreens;
}

void SimulatorRenderer::setScreenData(int screenId, Color *screenData) {
    if (screenId < screens.size()) {
        screens.at(screenId)->setScreenData(screenData);
    }
}

void SimulatorRenderer::render() {
    auto setScreenMessage = std::make_shared<matrixserver::MatrixServerMessage>();
    setScreenMessage->set_messagetype(matrixserver::setScreenFrame);
    int i = 0;
    for (auto screen : screens) {
        auto screenData = setScreenMessage->add_screendata();
        screenData->set_screenid(screen->getScreenId());
        screenData->set_framedata((char *) screen->getScreenData().data(), screen->getScreenDataSize()* sizeof(Color));
        screenData->set_encoding(matrixserver::ScreenData_Encoding_rgb24bbp);
    }
    connection->sendMessage(setScreenMessage);
}

void SimulatorRenderer::setGlobalBrightness(int brightness) {
    globalBrightness = brightness;
}

int SimulatorRenderer::getGlobalBrightness() {
    return globalBrightness;
}
