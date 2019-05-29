#include "SerialPort.h"

SerialPort::SerialPort(std::string port, unsigned int baud_rate)
        : io(), serial(io,port)
{
    serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
}

void SerialPort::writeString(std::string s)
{
    boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
}

std::string SerialPort::readLine()
{
    using namespace boost;
    char c;
    std::string result;
    for(;;)
    {
        asio::read(serial,asio::buffer(&c,1));
        switch(c)
        {
            case '\r':
                break;
            case '\n':
                return result;
            default:
                result+=c;
        }
    }
}
