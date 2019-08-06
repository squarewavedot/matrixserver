#include "ImuTest.h"

ImuTest::ImuTest() : CubeApplication(30){
    Imu.init();
}

bool ImuTest::loop() {
    static int loopcount = 0;
    std::cout << Imu.getAcceleration() << std::endl;
    fade(0.85);
    setPixel3D(Imu.getCubeAccIntersect(), Color::green());
    render();
    loopcount++;
    return true;
}
