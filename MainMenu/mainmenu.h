#ifndef SNAKE_PIXELFLOW_H
#define SNAKE_PIXELFLOW_H

#include "CubeApplication.h"

#include "Joystick.h"
#include "ADS1000.h"

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

class MainMenu : public CubeApplication{
public:
    MainMenu();
    bool loop();
private:
    std::vector<Joystick *> joysticks;
    std::vector<fs::path> executables;
    std::string searchDirectory;
    ADS1000 adcBattery;
    std::vector<Color> allTheColorsRainbow;
};

#endif //SNAKE_PIXELFLOW_H
