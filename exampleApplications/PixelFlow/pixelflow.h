#ifndef SNAKE_PIXELFLOW_H
#define SNAKE_PIXELFLOW_H

#include "CubeApplication.h"
#include "Joystick.h"
#include <Mpu6050.h>

class PixelFlow : public CubeApplication{
public:
    PixelFlow();
    bool loop();
private:
    Mpu6050 Imu;
    class Particle;
    class SurfaceParticle;
    class Drop;
};

class PixelFlow::Particle{
public:
    Particle(Vector3f pos, Vector3f vel, Vector3f accel, Color col);
    void step();
    void accelerate();
    void move();

    Vector3f position();
    Vector3f velocity();
    Vector3f acceleration();

    Vector3i iPosition();
    Vector3i iVelocity();
    Vector3i iAcceleration();

    void position(Vector3f pos);
    void velocity(Vector3f vel);
    void acceleration(Vector3f accel);

    Color color();
    void color(Color Col);
protected:
    Vector3f position_;
    Vector3f velocity_;
    Vector3f acceleration_;
    Color color_;
};

class PixelFlow::SurfaceParticle  : public Particle {
public:
    SurfaceParticle(Vector3i maxPos, Vector3f pos, Vector3f vel, Vector3f accel, Color col);
    void step();
    void warp();
    void accelerateOnSurface();
protected:
    Vector3i maxPosition;
    EdgeNumber lastEdge;
    Vector3i lastIPosition;
};

class PixelFlow::Drop : public SurfaceParticle {
public:
    Drop(Vector3i maxPos, Vector3f pos, Vector3f vel, Vector3f accel, Color col);
    void step();
    bool getRdyDelete();
private:
    float vxOld_;
    float vyOld_;
    Vector3i maxPos_;
    int stepCount;
    bool rdyDelete_;
};


#endif //SNAKE_PIXELFLOW_H
