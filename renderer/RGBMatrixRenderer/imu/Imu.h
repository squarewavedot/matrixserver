#ifndef __IMU__
#include "SerialPort.h"
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

class Imu{
public:
  Imu();
  bool init(std::string portname);
  float * getAcceleration();
  void startRefreshThread();
  void stopThread();
  void refresh();
  bool isInFreefall();
protected:
  std::vector<float> parseString(std::string input);
private:
  void internalLoop();
  float acceleration[3];
  SerialPort *serial;
  boost::thread * thread_;
  boost::mutex threadLock_;
  bool freefall;
  float threshold = 0.1;
};
#endif
