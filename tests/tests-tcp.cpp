#include "catch.hpp"

#include <TcpClient.h>
#include <TcpServer.h>
#include <SocketConnection.h>
#include <Screen.h>
#include <SimulatorRenderer.h>

//TEST_CASE("simulator"){
//    std::vector<std::shared_ptr<Screen>> screens;
//    for(int i = 0; i<6; i++)
//        screens.push_back(std::make_shared<Screen>(64,64,i));
//    auto renderer = std::make_shared<SimulatorRenderer>(screens, "neo.local");
//    for(auto screen: screens){
//        screen->fill(Color::green());
//    }
//    while(1){
//        std::cout << "render" << std::endl;
//        renderer->render();
//        renderer->render();
//        sleep(1);
//    }
//}