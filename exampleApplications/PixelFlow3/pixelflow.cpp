#include "pixelflow.h"
#include <cmath>

#include <iostream>
#include <algorithm>
#include <cctype>
#include <memory>

#define PI 3.14159265

PixelFlow::PixelFlow() : CubeApplication(40) {
    joysticks.push_back(new Joystick(0));
    joysticks.push_back(new Joystick(1));
    joysticks.push_back(new Joystick(2));
    joysticks.push_back(new Joystick(3));
}

bool PixelFlow::loop(){
    static std::vector<std::shared_ptr<Drop>> rdrops;
    static int counter = 0;
    static int counterColChange = 0;
    static Color col1(0,255-rand()%100,255-rand()%200);
    static bool isPaused = false;


    for (auto joystick : joysticks) {
        if (joystick->getButtonPress(0)) {
            counterColChange++;
        }
        if (joystick->getButtonPress(3)) {
            isPaused = !isPaused;
        }
        joystick->clearAllButtonPresses();
    }

    if(isPaused)
        return true;


    fade(0.85);
    //create new Raindrops
    for (int foo = 0; foo < 30; foo++){
        float randAngle = rand()%360;
        float vx = 0.5 * cos(randAngle*PI/180);
        float vy = 0.5 * sin(randAngle*PI/180);
        rdrops.push_back(std::make_shared<Drop>(Vector3i(VIRTUALCUBEMAXINDEX,VIRTUALCUBEMAXINDEX,VIRTUALCUBEMAXINDEX), Vector3f(VIRTUALCUBECENTER,VIRTUALCUBECENTER,0), Vector3f(vx,vy,0), Vector3f(0,0,0),col1));
    }

//    if (counter%50 == 0) {
//        counterColChange++;
//    }


    switch (counterColChange%2) {
        case 0:
            col1.r((uint8_t)0);
            col1.g((uint8_t)255);
            col1.b((uint8_t)150);
            col1 *= (float)((rand()%100))/100.0f;
            break;
        case 1:
            col1.g((uint8_t)(0));
            col1.b((uint8_t)(255-rand()%100));
            col1.r((uint8_t)(255-rand()%200));
            break;
        case 2:
            col1.b((uint8_t)(0));
            col1.r((uint8_t)(255-rand()%100));
            col1.g((uint8_t)(255-rand()%200));
            break;
        case 3:
            col1.r((uint8_t)(0));
            col1.g((uint8_t)(0));
            col1.b((uint8_t)(255-rand()%200));
            break;
        case 4:
            col1.g((uint8_t)(0));
            col1.b((uint8_t)(0));
            col1.r((uint8_t)(255-rand()%200));
            break;
        case 5:
            col1.b((uint8_t)(0));
            col1.r((uint8_t)(0));
            col1.g((uint8_t)(255-rand()%200));
            break;
    }

    for(auto r : rdrops) {
        if (counter%1 == 0) {
            r->step();
        }
        //setPixelSmooth3D(r->position(), r->color());
        setPixel3D(r->iPosition(), r->color());
    }

    //remove drops from the bottom
    rdrops.erase(std::remove_if(rdrops.begin(),rdrops.end(),[](std::shared_ptr<Drop> r){return (r->getRdyDelete());}),rdrops.end());

    render();
    counter++;

    return true;
}





PixelFlow::Drop::Drop(Vector3i maxPos, Vector3f pos, Vector3f vel, Vector3f accel, Color col)
        : Particle(pos, vel, accel, col){
    maxPos_ = maxPos;
    vxOld_ = 0.0f;
    vyOld_ = 0.0f;
    rdyDelete_ = false;
}

void PixelFlow::Drop::step(){
    Particle::step(); //Do the physics

    if(position_[0] < 0 || position_[1] < 0 || position_[0] > maxPos_[0] || position_[1] > maxPos_[1]) {
        velocity_[2] = 0.3;
        acceleration_[2] = 0.02+((float)(rand()%10)/400.0f);
        acceleration_[1] = 0;
        acceleration_[0] = 0;
        if (vxOld_ == 0 && vyOld_ == 0){
            vxOld_ = velocity_[0];
            vyOld_ = velocity_[1];
        }
        velocity_[0] = 0;
        velocity_[1] = 0;
    }

    if(position_[0] < 0) {
        position_[0] = 0;
        position_[2] = 0;
    }

    if(position_[1] < 0){
        position_[1] = 0;
        position_[2] = 0;
    }

    if(position_[0] > maxPos_[0]){
        position_[0] = maxPos_[0];
        position_[2] = 0;
    }

    if(position_[1] > maxPos_[1]){
        position_[1] = maxPos_[1];
        position_[2] = 0;
    }

    if(position_[2] < 0){
        position_[2] = 0;
        velocity_[2] *= -1;
    }

    if(position_[2] > maxPos_[2]){
        position_[2] = maxPos_[2];
        velocity_[0] = vxOld_ * -1;
        velocity_[1] = vyOld_ * -1;
        velocity_[2] = 0;
        acceleration_[1] = 0;
        acceleration_[0] = 0;
        acceleration_[2] = 0;
    }
    if(((velocity_[0] > 0 && position_[0] >VIRTUALCUBECENTER) || (velocity_[0]<0 && position_[0] < VIRTUALCUBECENTER)) && position_[2] == maxPos_[2]){
        velocity_[0] = 0;
        vxOld_ = 0;
    }
    if(((velocity_[1] > 0 && position_[1] >VIRTUALCUBECENTER) || (velocity_[1]<0 && position_[1] < VIRTUALCUBECENTER)) && position_[2] == maxPos_[2]){
        velocity_[1] = 0;
        vyOld_ = 0;
    }
    if (velocity_[0] == 0 && velocity_[1] == 0  && position_[2] == maxPos_[2]){
        rdyDelete_ = true;
    }

}

bool PixelFlow::Drop::getRdyDelete(){
    return rdyDelete_;
}


PixelFlow::Particle::Particle(Vector3f pos, Vector3f vel, Vector3f accel, Color col)
        :position_(pos),
         velocity_(vel),
         acceleration_(accel),
         color_(col){}

void PixelFlow::Particle::step(){
    accelerate();
    move();
}

void PixelFlow::Particle::move(){
    position_ += velocity_;
}

void PixelFlow::Particle::accelerate(){
    velocity_ += acceleration_;
}

Vector3f PixelFlow::Particle::position(){
    return position_;
}

Vector3f PixelFlow::Particle::velocity(){
    return velocity_;
}

Vector3f PixelFlow::Particle::acceleration(){
    return acceleration_;
}

Vector3i PixelFlow::Particle::iPosition(){
    return Vector3i(round(position()[0]),round(position()[1]),round(position()[2]));
}

Vector3i PixelFlow::Particle::iVelocity(){
    return Vector3i(round(velocity()[0]),round(velocity()[1]),round(position()[2]));
}

Vector3i PixelFlow::Particle::iAcceleration(){
    return Vector3i(round(acceleration()[0]),round(acceleration()[1]),round(acceleration()[2]));
}

void PixelFlow::Particle::position(Vector3f pos){
    position_ = pos;
}
void PixelFlow::Particle::velocity(Vector3f vel){
    velocity_ = vel;
}
void PixelFlow::Particle::acceleration(Vector3f accel){
    acceleration_ = accel;
}

Color PixelFlow::Particle::color(){
    return color_;
}
void PixelFlow::Particle::color(Color Col){
    color_ = Col;
}

