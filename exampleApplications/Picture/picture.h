#ifndef PICTURE_H
#define PICTURE_H

#include "CubeApplication.h"
#include "Joystick.h"
#include "Image.h"
#include <vector>

class Picture : public CubeApplication{
public:
    Picture();
    bool loop();

private:
    Image autoload;
    std::vector<Joystick *> joysticks;
};


#endif //PICTURE_H