#include "ADS1000.h"

#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>
#include <byteswap.h>

// values
// 944 = 10.05V
// 1412 = 15.02V
// 1613 = 17.15V
// 632 = 6.71V
// linear interpoliert: voltage = 0,0106 * value - 0,0055

#define ADS1000_I2C_ADDRESS        0x48   // I2C


int ADS1000::read_word_2c(int fd, char reg){
    uint16_t value = wiringPiI2CReadReg16(fd, reg);
    value = __bswap_16(value);
    if (value >= 0x8000)
        return -((65535 - value) + 1);
    else
        return value;
}

ADS1000::ADS1000(){
    init();
}

void ADS1000::init()
{
    fd = wiringPiI2CSetup(ADS1000_I2C_ADDRESS);
    if (fd == -1)
        return;

    int readval = read_word_2c(fd, 0x00); // do one dummy read

    startRefreshThread();
}

void ADS1000::startRefreshThread()
{
    thread_ = new boost::thread(&ADS1000::internalLoop, this);
}

void ADS1000::internalLoop(){
    int loopcount = 0;
    while(1){
        int readval = read_word_2c(fd, 0x00);
        voltage = (0.0106f * readval) - 0.0055f;
        usleep(100000); //10fps
    }
}

float ADS1000::getVoltage(){
    return voltage;
}