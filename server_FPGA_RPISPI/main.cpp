#include <iostream>
#include <fstream>
#include <vector>
#include <boost/log/trivial.hpp>

#include <Server.h>
#include <FPGARendererFTDI.h>
#include <FPGARendererRPISPI.h>
#include <TcpServer.h>

#include <matrixserver.pb.h>
#include <google/protobuf/util/json_util.h>

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

void handleServerConfig(int argc, char **argv, matrixserver::ServerConfig &serverConfig) {
    if (argc == 2) {
        BOOST_LOG_TRIVIAL(debug) << "[Server] Trying to read config from: " << argv[1];
        std::ifstream configFileReadStream(argv[1]);
        std::stringstream buffer;
        buffer << configFileReadStream.rdbuf();
        if (google::protobuf::util::JsonStringToMessage(buffer.str(), &serverConfig).ok()) {
            BOOST_LOG_TRIVIAL(debug) << "[Server] ServerConfig successfully read from: " << argv[1];
        } else {
            BOOST_LOG_TRIVIAL(debug) << "[Server] ServerConfig read failed from: " << argv[1];
        }
    } else {
        BOOST_LOG_TRIVIAL(debug) << "[Server] creating default config";
        createDefaultCubeConfig(serverConfig);
        std::string configString;
        google::protobuf::util::JsonOptions jsonOptions;
        jsonOptions.add_whitespace = true;
        jsonOptions.always_print_primitive_fields = true;
        if (google::protobuf::util::MessageToJsonString(serverConfig, &configString, jsonOptions).ok()) {
            std::string configFileName = "matrixServerConfig.json";
            std::ofstream configFileWriteStream(configFileName, std::ios_base::trunc);
            configFileWriteStream << configString;
            configFileWriteStream.close();
            BOOST_LOG_TRIVIAL(debug) << "[Server] written default config to " << configFileName;
        }

    }
}

int main(int argc, char **argv) {
    matrixserver::ServerConfig serverConfig;
    handleServerConfig(argc, argv, serverConfig);

    BOOST_LOG_TRIVIAL(info) << "ServerConfig: " << std::endl << serverConfig.DebugString() << std::endl;

    std::vector<std::shared_ptr<Screen>> screens;
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


    auto rendererFPGA = std::make_shared<FPGARendererRPISPI>(screens);
//    auto rendererSimulator = std::make_shared<SimulatorRenderer>(screens, "10.42.42.69");

    Server server(rendererFPGA, serverConfig);
//    server.addRenderer(rendererSimulator);

    while (server.tick()) {
        usleep(100000);
    };
    return 0;
}