#include "picture.h"
#include <cmath>

#include <iostream>
#include <algorithm>
#include <cctype>
#include <memory>


std::string filepath = "/media/usb0/autoload.png";


Picture::Picture(){
    joysticks.push_back(new Joystick(0));
    joysticks.push_back(new Joystick(1));
    joysticks.push_back(new Joystick(2));
    joysticks.push_back(new Joystick(3));

    autoload.loadImage(filepath.data());
    if(autoload.getWidth() == 384 && autoload.getHeight()%64 == 0)
        std::cout << "autoload image " << filepath << " successful, size: " << autoload.getWidth() << "x" << autoload.getHeight() << std::endl;
}

bool Picture::loop(){
    static int loopcount = 0;
    static int verticalPos = 0;

    clear();

    for (auto joystick : joysticks) {
        if (joystick->getButtonPress(0)) {
            verticalPos += 64;
            if(verticalPos > autoload.getHeight()-64)
                verticalPos = 0;
            std::cout << "Verticalpos: " << verticalPos << std::endl;
        }
        joystick->clearAllButtonPresses();
    }

    drawImage(top, Vector2i(0, 0), autoload, Vector2i(0, verticalPos));
    drawImage(left, Vector2i(0, 0), autoload, Vector2i(64, verticalPos));
    drawImage(front, Vector2i(0, 0), autoload, Vector2i(128, verticalPos));
    drawImage(right, Vector2i(0, 0), autoload, Vector2i(192, verticalPos));
    drawImage(back, Vector2i(0, 0), autoload, Vector2i(256, verticalPos));
    drawImage(bottom, Vector2i(0, 0), autoload, Vector2i(320, verticalPos));

    loopcount++;
    render();
    return true;
}
