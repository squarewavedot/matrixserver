#ifndef SNAKE_PIXELFLOW_H
#define SNAKE_PIXELFLOW_H

#include "CubeApplication.h"

#include "Joystick.h"
#include "ADS1000.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class MainMenu : public CubeApplication {
public:
    MainMenu();

    bool loop();

private:
    class AppListItem;

    std::vector<Joystick *> joysticks;
    std::vector<AppListItem> appList;
    std::string searchDirectory;
    ADS1000 adcBattery;
};

class MainMenu::AppListItem {
public:
    AppListItem(std::string setName, std::string setExecPath);

    AppListItem(std::string setName, std::string setExecPath, Color setColor);

    std::string name;
    std::string execPath;
    bool isInternal = false;
    Color color = Color::white();
};

#endif //SNAKE_PIXELFLOW_H
