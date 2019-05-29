#include "Imu.h"
#include <cmath>

Imu::Imu(){

}

bool Imu::init(std::string portname){
  serial = new SerialPort(portname.c_str(), 115200);
  startRefreshThread();
  return true;
}

std::vector<float> Imu::parseString(std::string input){
    std::vector<float> result;
    std::istringstream sstream(input);
    for (std::string part; std::getline(sstream, part, ','); ) {
        result.push_back(std::stof(part));
    }
    return result;
}

float * Imu::getAcceleration(){
    return acceleration;
}

bool Imu::isInFreefall(){
  return freefall;
}

void Imu::refresh()
{
  std::vector<float> result = parseString(serial->readLine());
  if(result.size() == 3){
    acceleration[0] = result[0];
    acceleration[1] = result[1];
    acceleration[2] = result[2];
    if(std::abs(acceleration[0]) < threshold && std::abs(acceleration[1]) < threshold && std::abs(acceleration[2]) < threshold)
      freefall = true;
    else
      freefall = false;
  }
}

void Imu::startRefreshThread()
{
  thread_ = new boost::thread(&Imu::internalLoop, this);
}

void Imu::stopThread()
{
  if(thread_ != NULL)
  {
	thread_->interrupt();
    thread_->join();
    thread_ = NULL;
  }
}

void Imu::internalLoop()
{
  while(1){
    refresh();
    usleep(1000);
  }
}
