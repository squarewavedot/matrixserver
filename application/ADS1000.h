#ifndef MATRIXSERVER_ADS1000_H
#define MATRIXSERVER_ADS1000_H

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

class ADS1000 {
public:
    ADS1000();
    void init();
    float getVoltage();
private:
    void startRefreshThread();
    void internalLoop();
    int read_word_2c(int fd, char reg);
    boost::thread * thread_;
    boost::mutex threadLock_;
    int fd;

    float voltage;
};


#endif //MATRIXSERVER_ADS1000_H
