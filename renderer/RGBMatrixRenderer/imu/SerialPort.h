#ifndef __SERIALPORT__
#include <iostream>
#include <string>
#include <sstream>
#include <boost/asio.hpp>

class SerialPort
{
public:
  SerialPort(std::string port, unsigned int baud_rate);
  void writeString(std::string s);
  std::string readLine();
private:
  void internalLoop();
  boost::asio::io_service io;
  boost::asio::serial_port serial;
};

#endif
