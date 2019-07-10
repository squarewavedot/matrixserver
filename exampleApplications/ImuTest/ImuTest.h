#ifndef MATRIXSERVER_CUBETEST_H
#define MATRIXSERVER_CUBETEST_H

#include <CubeApplication.h>
#include <Mpu6050.h>

class ImuTest : public CubeApplication{
public:
    ImuTest();
    bool loop();
private:
    Mpu6050 Imu;
};

#endif //MATRIXSERVER_CUBETEST_H
