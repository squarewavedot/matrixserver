#ifndef MATRIXSERVER_JOYSTICK_H
#define MATRIXSERVER_JOYSTICK_H

#include <string>
#include <iostream>
#include <stdint.h>
#include <map>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#define MAXBUTTONAXISCOUNT 16

class Joystick
{
  public:
    class Event;
    ~Joystick();
    Joystick();
    Joystick(int joystickNumber);
    Joystick(std::string devicePath);
    Joystick(Joystick const&) = delete;
    Joystick(Joystick &&) = default;
    Joystick(std::string devicePath, bool blocking);
    void init(std::string devicePath, bool blocking);
    bool isFound();
    bool sample(Joystick::Event * event);
    void startRefreshThread();
    void stopThread();
    void refresh();
    bool getButton(unsigned int num);
    bool getButtonPress(unsigned int num);
    float getAxis(unsigned int num);
    void clearAllButtonPresses();
  private:
    void internalLoop();
    void openPath();
    void recheckFilePath();
    void resetVariables();
    int _fd;
    boost::thread * thread_;
    boost::mutex threadLock_;
    std::string devicePath_;
    bool blocking_;
    std::array<bool, MAXBUTTONAXISCOUNT> button_;
    std::array<bool, MAXBUTTONAXISCOUNT> buttonPress_;
    std::array<float, MAXBUTTONAXISCOUNT> axis_;
};

class Joystick::Event
{
  public:
    unsigned int time;
    short value;
    unsigned char type;
    unsigned char number;
    bool isButton()
    {
      return (type & 0x01) != 0;
    }

    bool isAxis()
    {
      return (type & 0x02) != 0;
    }

    bool isInitialState()
    {
      return (type & 0x80) != 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Joystick::Event& e);
};

std::ostream& operator<<(std::ostream& os, const Joystick::Event& e);


#endif //MATRIXSERVER_JOYSTICK_H
