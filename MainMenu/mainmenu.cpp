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

enum MenuState {
    applist, settings, settingsUpdate
};

MenuState menuState = applist;

MainMenu::MainMenu() : CubeApplication(40) {
    searchDirectory = "/home/pi/APPS";
    for (const auto &p : std::experimental::filesystem::directory_iterator(searchDirectory)) {
        //if(p.path().extension() == "cube"){
        appList.push_back(AppListItem(std::string(p.path().filename()), std::string(p.path())));
        //}
    }

    appList.push_back(AppListItem("settings", "settings", true, Color::blue()));
    settingsList.push_back(AppListItem("update", "update", true));
    settingsList.push_back(AppListItem("return", "return", true, Color::blue()));

    joysticks.push_back(new Joystick(0));
    joysticks.push_back(new Joystick(1));
    joysticks.push_back(new Joystick(2));
    joysticks.push_back(new Joystick(3));

    gethostname(hostname, hostnameLen);
    menuState = applist;
}


bool MainMenu::loop() {
    static int selectedExec = appList.size() / 2;
    static float lastAxisOne = 0;
    static int loopcount = 0;

    static Color colSelectedText = Color::white();
    static Color colVoltageText = Color::blue();

    clear();

    colSelectedText.fromHSV((loopcount % 360 / 1.0f), 1.0, 1.0);

    auto battValue = adcBattery.getVoltage();
    for (uint screenCounter = 0; screenCounter < 4; screenCounter++) {
        drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::right, 58), colVoltageText, std::to_string(battValue).substr(0, 5) + " V");
        drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::left, 1), colVoltageText, hostname);
    }

    switch (menuState) {
        case applist: {
            float axisOne = joysticks[0]->getAxis(1);
            if (axisOne == 0) axisOne = joysticks[1]->getAxis(1);
            if (axisOne == 0) axisOne = joysticks[2]->getAxis(1);
            if (axisOne == 0) axisOne = joysticks[3]->getAxis(1);

            if (axisOne != 0 && lastAxisOne == 0) {
                selectedExec += (int) axisOne;
            }
            if (selectedExec < 0) {
                selectedExec = appList.size() - 1;
            } else {
                selectedExec %= appList.size();
            }

            lastAxisOne = axisOne;
            //std::cout << "selectedExec: " << selectedExec << " appList.size(): " << appList.size() << std::endl;

            if (joysticks[0]->getButtonPress(0) ||
                joysticks[1]->getButtonPress(0) ||
                joysticks[2]->getButtonPress(0) ||
                joysticks[3]->getButtonPress(0)) {
                if (appList.at(selectedExec).execPath == "settings") {
                    selectedExec = 0;
                    menuState = settings;
                } else {
                    std::string temp = appList.at(selectedExec).execPath;
                    temp += std::string(" 1>/dev/null 2>/dev/null &");
                    temp = std::string("nohup ") + temp;
                    std::cout << "start: " << temp << std::endl;
                    system(temp.data());
                }

            }


            joysticks[0]->clearAllButtonPresses();
            joysticks[1]->clearAllButtonPresses();
            joysticks[2]->clearAllButtonPresses();
            joysticks[3]->clearAllButtonPresses();

            for (uint i = 0; i < appList.size(); i++) {
                int yPos = 29 + ((i - selectedExec) * 7);
                Color textColor = appList.at(i).color;
                if (i == (uint) selectedExec)
                    textColor = colSelectedText;
                for (uint screenCounter = 0; screenCounter < 4; screenCounter++) {
                    if (yPos < 57 && yPos > 6) {
                        drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::centered, yPos), textColor, appList.at(i).name);
                    }
                }
            }

            drawText(top, Vector2i(CharacterBitmaps::centered, 22), colSelectedText, "DOT");
            drawText(top, Vector2i(CharacterBitmaps::centered, 30), colSelectedText, "THE");
            drawText(top, Vector2i(CharacterBitmaps::centered, 38), colSelectedText, "LEDCUBE");
        }
            break;
        case settings: {
            drawText(top, Vector2i(CharacterBitmaps::centered, 30), Color::blue(), "Settings");


            float axisOne = joysticks[0]->getAxis(1);
            if (axisOne == 0) axisOne = joysticks[1]->getAxis(1);
            if (axisOne == 0) axisOne = joysticks[2]->getAxis(1);
            if (axisOne == 0) axisOne = joysticks[3]->getAxis(1);

            if (axisOne != 0 && lastAxisOne == 0) {
                selectedExec += (int) axisOne;
            }
            if (selectedExec < 0) {
                selectedExec = settingsList.size() - 1;
            } else {
                selectedExec %= settingsList.size();
            }

            lastAxisOne = axisOne;

            if (joysticks[0]->getButtonPress(0) ||
                joysticks[1]->getButtonPress(0) ||
                joysticks[2]->getButtonPress(0) ||
                joysticks[3]->getButtonPress(0)) {
                if (settingsList.at(selectedExec).execPath == "return") {
                    menuState = applist;
                    selectedExec = appList.size()-1;
                } else if (settingsList.at(selectedExec).execPath == "update") {
                    menuState = settingsUpdate;
                    auto temp = std::string("/usr/local/sbin/Update.sh 1>/dev/null 2>/dev/null &");
                    temp = std::string("nohup ") + temp;
                    system(temp.data());
                }

            }


            joysticks[0]->clearAllButtonPresses();
            joysticks[1]->clearAllButtonPresses();
            joysticks[2]->clearAllButtonPresses();
            joysticks[3]->clearAllButtonPresses();

            for (uint i = 0; i < settingsList.size(); i++) {
                int yPos = 29 + ((i - selectedExec) * 7);
                Color textColor = settingsList.at(i).color;
                if (i == (uint) selectedExec)
                    textColor = colSelectedText;
                for (uint screenCounter = 0; screenCounter < 4; screenCounter++) {
                    if (yPos < 57 && yPos > 6) {
                        drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::centered, yPos), textColor, settingsList.at(i).name);
                    }
                }
            }
            }
            break;
        case settingsUpdate:
            drawText(top, Vector2i(CharacterBitmaps::centered, 22), colSelectedText, "Update");
            drawText(top, Vector2i(CharacterBitmaps::centered, 30), colSelectedText, "in");
            drawText(top, Vector2i(CharacterBitmaps::centered, 38), colSelectedText, "progress");
            for (uint screenCounter = 0; screenCounter < 4; screenCounter++) {
                drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::centered, 22), colSelectedText, "Update");
                drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::centered, 30), colSelectedText, "in");
                drawText((ScreenNumber) screenCounter, Vector2i(CharacterBitmaps::centered, 38), colSelectedText, "progress");
            }
            break;

    }


    render();
    loopcount++;
    return true;
}

MainMenu::AppListItem::AppListItem(std::string setName, std::string setExecPath, bool setInternal, Color setColor) {
    name = setName;
    execPath = setExecPath;
    color = setColor;
    isInternal = setInternal;
};