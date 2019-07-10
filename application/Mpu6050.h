#ifndef MATRIXSERVER_MPU6050_H
#define MATRIXSERVER_MPU6050_H

#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

class Mpu6050 {
public:
    Mpu6050();
    void init();
    Eigen::Vector3i getCubeAccIntersect();
    Eigen::Vector3f getAcceleration();
private:
    void startRefreshThread();
    void internalLoop();
    boost::thread * thread_;
    boost::mutex threadLock_;
    int fd;

    Eigen::Vector3f acceleration;
};


#endif //MATRIXSERVER_MPU6050_H
