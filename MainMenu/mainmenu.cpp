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
#include <unistd.h>

size_t hostnameLen = 20;
char hostname[20];


Color ColorFade(Color col1, Color col2, float progress){
    Color returnColor;
    returnColor.r((uint8_t)(col1.r() + (col2.r() - col1.r()) * progress/100));
    returnColor.g((uint8_t)(col1.g() + (col2.g() - col1.g()) * progress/100));
    returnColor.b((uint8_t)(col1.b() + (col2.b() - col1.b()) * progress/100));

    return returnColor;
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


    allTheColorsRainbow.push_back(Color(255,0,0));
    allTheColorsRainbow.push_back(Color(255,255,0));
    allTheColorsRainbow.push_back(Color(0,255,0));
    allTheColorsRainbow.push_back(Color(0,255,255));
    allTheColorsRainbow.push_back(Color(0,0,255));
    allTheColorsRainbow.push_back(Color(255,0,255));

    gethostname(hostname, hostnameLen);
    std::cout << hostname << std::endl;
}


bool MainMenu::loop(){
    static int selectedExec = executables.size()/2;
    static float lastAxisOne = 0;
    static int loopcount = 0;



    static int countRainbow = 0;
    static int colorFadeNumber = 0;
    static Color colNormalText=Color(255,0,0);
    static Color colSelectedText=Color::green();
    static Color colTopText=Color::white();
    static Color colVoltageText=Color::blue();
    static Color col1RainbowOld=colNormalText;
    static Color col1RainbowNew=Color(255,255,0);
    static int menuColorMode = 2;
    static int maxColorMode = 2;


    clear();

    float axisOne = joysticks[0]->getAxis(1);
    if(axisOne == 0) axisOne = joysticks[1]->getAxis(1);
    if(axisOne == 0) axisOne = joysticks[2]->getAxis(1);
    if(axisOne == 0) axisOne = joysticks[3]->getAxis(1);
    if(axisOne != 0 && lastAxisOne == 0){
        selectedExec += (int)axisOne;
    }
    lastAxisOne = axisOne;
    if (selectedExec<0){
        selectedExec = executables.size()-1;
    }else{
        selectedExec %= executables.size();
    }
   //std::cout << "selectedExec: " << selectedExec << " executables.size(): " << executables.size() << std::endl;

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

    if(joysticks[0]->getButtonPress(7) ||
       joysticks[1]->getButtonPress(7) ||
       joysticks[2]->getButtonPress(7) ||
       joysticks[3]->getButtonPress(7)){
        menuColorMode++;
    }

    if(joysticks[0]->getButtonPress(6) ||
       joysticks[1]->getButtonPress(6) ||
       joysticks[2]->getButtonPress(6) ||
       joysticks[3]->getButtonPress(6)){
        menuColorMode--;
    }

    joysticks[0]->clearAllButtonPresses();
    joysticks[1]->clearAllButtonPresses();
    joysticks[2]->clearAllButtonPresses();
    joysticks[3]->clearAllButtonPresses();

    auto battValue = adcBattery.getVoltage();

// Color change Mode
    countRainbow++;
    if (countRainbow >= 100){
        countRainbow = 0;
        colorFadeNumber++;
        col1RainbowOld = col1RainbowNew;
    }
    if (colorFadeNumber >= allTheColorsRainbow.size()){
        colorFadeNumber = 0;
    }
    col1RainbowNew = allTheColorsRainbow.at(colorFadeNumber);

switch(menuColorMode) {
    case 0:
        colNormalText = Color::white();
        colSelectedText = Color::green();
        colTopText=Color::white();
        colVoltageText=Color::blue();
        break;
    case 1:
        colNormalText = ColorFade(col1RainbowOld,col1RainbowNew,countRainbow);
        colSelectedText = colNormalText + Color(127,127,127);
        colTopText=colNormalText;
        colVoltageText=Color::blue();
        break;
    case 2:
        colNormalText = Color::white();
        colSelectedText = ColorFade(col1RainbowOld,col1RainbowNew,countRainbow);
        colTopText=colSelectedText;
        colVoltageText=Color::blue();
        break;
    default:
        if (menuColorMode < 0){
            menuColorMode = maxColorMode;
        }
        if (menuColorMode > maxColorMode){
            menuColorMode = 0;
        }
        break;
}
   // std::cout << "countRainbow: " << countRainbow << " colorFadeNumber: " << colorFadeNumber << std::endl;

    for(uint i = 0; i < executables.size(); i++){
        int yPos = 29+((i-selectedExec)*7);
        Color textColor = colNormalText;
        if(i == (uint)selectedExec)
            textColor = colSelectedText;
        for(uint screenCounter = 0; screenCounter < 4; screenCounter++){
            if (yPos < 57 && yPos > 6){
                drawText((ScreenNumber)screenCounter, Vector2i(CharacterBitmaps::centered, yPos), textColor, executables.at(i).filename());
            }
        }
    }

    for(uint screenCounter = 0; screenCounter < 4; screenCounter++){
        drawText((ScreenNumber)screenCounter, Vector2i(CharacterBitmaps::right, 58), colVoltageText, std::to_string(battValue).substr(0,5) + " V");
        drawText((ScreenNumber)screenCounter, Vector2i(CharacterBitmaps::left, 1), colVoltageText, hostname);
    }

    drawText(top, Vector2i(CharacterBitmaps::centered, 22), colTopText, "DOT");
    drawText(top, Vector2i(CharacterBitmaps::centered, 30), colTopText, "THE");
    drawText(top, Vector2i(CharacterBitmaps::centered, 38), colTopText, "LEDCUBE");

    //drawText(top, Vector2i(CharacterBitmaps::centered, 58), Color::green()+Color::blue(), "squarewave.io");
    render();

    loopcount++;
    return true;
}
