#ifndef SNAKE_PIXELFLOW_H
#define SNAKE_PIXELFLOW_H

#include "CubeApplication.h"

#include "Joystick.h"
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
};

#endif //SNAKE_PIXELFLOW_H
