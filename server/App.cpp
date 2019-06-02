#include <iostream>
#include <boost/log/trivial.hpp>
#include "App.h"

App::App(std::shared_ptr<UniversalConnection> setCon) {
    connection = setCon;
    appId = generateAppId();
}

int App::getAppId() {
    return appId;
}

void App::sendMsg(std::shared_ptr<matrixserver::MatrixServerMessage> message){
    connection->sendMessage(message);
}

void App::setAppState(AppState newAppState){
    appState = newAppState;
}

AppState App::getAppState(){
    return appState;
}

std::shared_ptr<UniversalConnection> App::getConnection() {
    return connection;
}

int App::generateAppId() {
    //todo implement check for duplicates
    return rand();
}