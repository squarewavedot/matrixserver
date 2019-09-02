#include "Joystick.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include "unistd.h"

Joystick::Joystick()
{
    init("/dev/input/js0", false);
}

Joystick::Joystick(int joystickNumber)
{
    std::stringstream sstm;
    sstm << "/dev/input/js" << joystickNumber;
    init(sstm.str(), false);
}

Joystick::Joystick(std::string devicePath)
{
    init(devicePath, false);
}

Joystick::Joystick(std::string devicePath, bool blocking)
{
    init(devicePath, blocking);
}

void Joystick::resetVariables(){
    for(int i = 0; i < MAXBUTTONAXISCOUNT; i++){
        buttonPress_[i] = 0;
        button_[i] = 0;
        axis_[i] = 0;
    }
}

void Joystick::init(std::string devicePath, bool blocking){
    resetVariables();
    devicePath_ = devicePath;
    blocking_ = blocking;
    openPath();
    startRefreshThread();
}

void Joystick::openPath()
{
    struct stat buffer;
    if (stat (devicePath_.c_str(), &buffer) == 0){
        sleep(1);
        _fd = open(devicePath_.c_str(), blocking_ ? O_RDONLY : O_RDONLY | O_NONBLOCK);
    }
}

void Joystick::recheckFilePath()
{
    struct stat buffer;
    if (stat (devicePath_.c_str(), &buffer) != 0){
        _fd = 0;
    }
    if(_fd == 0){
        resetVariables();
        openPath();
    }
}

bool Joystick::sample(Joystick::Event * event)
{
    if(_fd > 0){
        int bytes = read(_fd, event, sizeof(*event));
        if (bytes == -1)
            return false;
        return bytes == sizeof(*event);
    }
    return false;
}

bool Joystick::isFound()
{
    struct stat buffer;
    return (stat (devicePath_.c_str(), &buffer) == 0);
}

void Joystick::refresh()
{
    Joystick::Event event;
    while (sample(&event))
    {
        if(event.number < MAXBUTTONAXISCOUNT){
            if (event.isButton()){
                if(!button_[event.number] && (bool)event.value)
                    buttonPress_[event.number] = true;
                button_[event.number] = (bool)event.value;
            }else if (event.isAxis()){
                axis_[event.number] = (float)event.value / INT16_MAX;
            }
        }
    }
}

void Joystick::startRefreshThread()
{
    thread_ = new boost::thread(&Joystick::internalLoop, this);
}

void Joystick::stopThread()
{
    if(thread_ != NULL)
    {
        thread_->interrupt();
        thread_->join();
        thread_ = NULL;
    }
}

void Joystick::internalLoop()
{
    int loopcount = 0;
    while(1){
        if(loopcount % 10 == 0)
            recheckFilePath();
        refresh();
        loopcount++;
        usleep(10000);
    }
}


bool Joystick::getButton(unsigned int num)
{
    if(num < MAXBUTTONAXISCOUNT)
        return button_[num];
    return false;
}

bool Joystick::getButtonPress(unsigned int num)
{
    if(num < MAXBUTTONAXISCOUNT){
        if(buttonPress_[num]){
            buttonPress_[num] = false;
            return true;
        }
    }
    return false;
}

float Joystick::getAxis(unsigned int num)
{
    if(num < MAXBUTTONAXISCOUNT)
        return axis_[num];
    return 0;
}

void Joystick::clearAllButtonPresses(){
    for(int i = 0; i < MAXBUTTONAXISCOUNT; i++){
        buttonPress_[i] = false;
    }
}

Joystick::~Joystick()
{
    stopThread();
    close(_fd);
}

std::ostream& operator<<(std::ostream& os, const Joystick::Event& e)
{
    os << "type=" << static_cast<int>(e.type)
       << " number=" << static_cast<int>(e.number)
       << " value=" << static_cast<int>(e.value);
    return os;
}
