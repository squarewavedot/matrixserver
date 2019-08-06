#include "mainmenu.h"

#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <cmath>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

MainMenu::MainMenu() : CubeApplication(40) {
    searchDirectory = "/home/pi/APPS";
    for (const auto & p : std::experimental::filesystem::directory_iterator(searchDirectory)){
        executables.push_back(p.path());
    }
    joysticks.push_back(new Joystick(0));
    joysticks.push_back(new Joystick(1));
    joysticks.push_back(new Joystick(2));
    joysticks.push_back(new Joystick(3));
}

bool MainMenu::loop(){
    static int selectedExec = executables.size()/2;
    static float lastAxisOne = 0;
    static int loopcount = 0;
    static std::string battValue;
    clear();

    float axisOne = joysticks[0]->getAxis(1);
    if(axisOne == 0) axisOne = joysticks[1]->getAxis(1);
    if(axisOne == 0) axisOne = joysticks[2]->getAxis(1);
    if(axisOne == 0) axisOne = joysticks[3]->getAxis(1);
    if(axisOne != 0 && lastAxisOne == 0){
        selectedExec += (int)axisOne;
    }
    lastAxisOne = axisOne;
    selectedExec %= executables.size();

    if(joysticks[0]->getButtonPress(0) ||
       joysticks[1]->getButtonPress(0) ||
       joysticks[2]->getButtonPress(0) ||
       joysticks[3]->getButtonPress(0)){
        std::string temp(executables.at(selectedExec));
        temp += std::string(" 1>/dev/null 2>/dev/null &");
        temp = std::string("nohup ") + temp;
        std::cout << "start: " << temp << std::endl;
        system(temp.data());
    }

    joysticks[0]->clearAllButtonPresses();

    for(uint i = 0; i < executables.size(); i++){
        Color textColor = Color::white();
        if(i == (uint)selectedExec)
            textColor = Color::green();
        for(uint screenCounter = 0; screenCounter < 4; screenCounter++)
            drawText((ScreenNumber)screenCounter, Vector2i(CharacterBitmaps::centered, 29+((i-selectedExec)*7)), textColor, executables.at(i).filename());
    }

    if(loopcount%40 == 0)
        battValue = exec("/home/pi/read_adc_battvoltage.py");

    drawText(top, Vector2i(CharacterBitmaps::centered, 22), Color::white(), "DOT");
    drawText(top, Vector2i(CharacterBitmaps::centered, 30), Color::white(), "THE");
    drawText(top, Vector2i(CharacterBitmaps::centered, 38), Color::white(), "LEDCUBE");
    drawText(top, Vector2i(CharacterBitmaps::right, 58), Color::blue(), battValue.substr(0,5) + " V");
    //drawText(top, Vector2i(CharacterBitmaps::centered, 58), Color::green()+Color::blue(), "squarewave.io");
    render();

    loopcount++;
    return true;
}
