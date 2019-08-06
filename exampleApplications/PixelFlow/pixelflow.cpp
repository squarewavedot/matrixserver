#include "pixelflow.h"
#include <cmath>

#include <iostream>
#include <algorithm>
#include <cctype>
#include <memory>

#define PI 3.14159265


EdgeNumber getEdgeNumberThis(Vector3i point) {
    EdgeNumber result = anyEdge;
    if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == 0)
        result = frontRight;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[1] == VIRTUALCUBEMAXINDEX)
        result = rightBack;
    else if (point[0] == 0 && point[1] == VIRTUALCUBEMAXINDEX)
        result = backLeft;
    else if (point[0] == 0 && point[1] == 0)
        result = leftFront;
    else if (point[1] == 0 && point[2] == 0)
        result = topFront;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[2] == 0)
        result = topRight;
    else if (point[1] == VIRTUALCUBEMAXINDEX && point[2] == 0)
        result = topBack;
    else if (point[0] == 0 && point[2] == 0)
        result = topLeft;
    else if (point[1] == 0 && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomFront;
    else if (point[0] == VIRTUALCUBEMAXINDEX && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomRight;
    else if (point[1] == VIRTUALCUBEMAXINDEX && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomBack;
    else if (point[0] == 0 && point[2] == VIRTUALCUBEMAXINDEX)
        result = bottomLeft;
    return result;
}

ScreenNumber getScreenNumberThis(Vector3i point) {
    ScreenNumber result = anyScreen;
    if (point[0] == 0)
    result = left;
    else if (point[0] == VIRTUALCUBEMAXINDEX)
    result = right;
    else if (point[1] == 0)
    result = front;
    else if (point[1] == VIRTUALCUBEMAXINDEX)
    result = back;
    else if (point[2] == 0)
    result = top;
    else if (point[2] == VIRTUALCUBEMAXINDEX)
    result = bottom;
    return result;
}

PixelFlow::PixelFlow() : CubeApplication(40) {
}

bool PixelFlow::loop(){
    static std::vector<std::shared_ptr<Drop>> rdrops;
    static int counter = 0;
    static int counterColChange = 0;
    static Color col1(0,255-rand()%100,255-rand()%200);


//    clear();
    fade(0.85);
    //create new Raindrops
    for (int foo = 0; foo < 60; foo++){
        float randAngle = rand()%360;
        float speed = 0;
        float vx = speed * cos(randAngle*PI/180);
        float vy = speed * sin(randAngle*PI/180);
        Vector3f startSpeed(0,0,0);
        auto imuPoint = Imu.getCubeAccIntersect();
        switch(getScreenNumber(imuPoint)){
            case ScreenNumber::top:
            case ScreenNumber::bottom:
                startSpeed[0] = vx;
                startSpeed[1] = vy;
                break;
            case ScreenNumber::front:
            case ScreenNumber::back:
                startSpeed[0] = vx;
                startSpeed[2] = vy;
                break;
            case ScreenNumber::left:
            case ScreenNumber::right:
                startSpeed[1] = vx;
                startSpeed[2] = vy;
                break;
            case ScreenNumber::anyScreen:
            default:
                break;
        }
        Vector3f startPoint = imuPoint.template cast<float>();
        rdrops.push_back(std::make_shared<Drop>(Vector3i(VIRTUALCUBEMAXINDEX,VIRTUALCUBEMAXINDEX,VIRTUALCUBEMAXINDEX), startPoint, startSpeed, Vector3f(0,0,0),col1));
    }

    if (counter%50 == 0) {
        counterColChange++;
    }
    switch (counterColChange%5) {
        case 0:
            col1.r((uint8_t)0);
            col1.g((uint8_t)(255-rand()%100));
            col1.b((uint8_t)(255-rand()%200));
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
            r->acceleration(Imu.getAcceleration() * (-0.1f+((float)(rand()%100)/2000.0f)));
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
        : SurfaceParticle(maxPos, pos, vel, accel, col){
    maxPos_ = maxPos;
    vxOld_ = 0.0f;
    vyOld_ = 0.0f;
    rdyDelete_ = false;
    stepCount = 0;
}

void PixelFlow::Drop::step(){
    SurfaceParticle::step(); //Do the physics

//    if(position_[0] < 0 || position_[1] < 0 || position_[0] > maxPos_[0] || position_[1] > maxPos_[1]) {
//        velocity_[2] = 0.5;
//        acceleration_[2] = 0.02+((float)(rand()%100)/1000.0f);
//        acceleration_[1] = 0;
//        acceleration_[0] = 0;
//        if (vxOld_ == 0 && vyOld_ == 0){
//            vxOld_ = velocity_[0];
//            vyOld_ = velocity_[1];
//        }
//        velocity_[0] = 0;
//        velocity_[1] = 0;
//    }
//
//    if(position_[0] < 0) {
//        position_[0] = 0;
//        position_[2] = 0;
//    }
//
//    if(position_[1] < 0){
//        position_[1] = 0;
//        position_[2] = 0;
//    }
//
//    if(position_[0] > maxPos_[0]){
//        position_[0] = maxPos_[0];
//        position_[2] = 0;
//    }
//
//    if(position_[1] > maxPos_[1]){
//        position_[1] = maxPos_[1];
//        position_[2] = 0;
//    }
//
//    if(position_[2] < 0){
//        position_[2] = 0;
//        velocity_[2] *= -1;
//    }
//
//    if(position_[2] > maxPos_[2]){
//        position_[2] = maxPos_[2];
//        velocity_[0] = vxOld_ * -1;
//        velocity_[1] = vyOld_ * -1;
//        velocity_[2] = 0;
//        acceleration_[1] = 0;
//        acceleration_[0] = 0;
//        acceleration_[2] = 0;
//    }
//    if(((velocity_[0] > 0 && position_[0] >VIRTUALCUBECENTER) || (velocity_[0]<0 && position_[0] < VIRTUALCUBECENTER)) && position_[2] == maxPos_[2]){
//        velocity_[0] = 0;
//        vxOld_ = 0;
//    }
//    if(((velocity_[1] > 0 && position_[1] >VIRTUALCUBECENTER) || (velocity_[1]<0 && position_[1] < VIRTUALCUBECENTER)) && position_[2] == maxPos_[2]){
//        velocity_[1] = 0;
//        vyOld_ = 0;
//    }
//    if (velocity_[0] == 0 && velocity_[1] == 0  && position_[2] == maxPos_[2]){
//        rdyDelete_ = true;
//    }
    if (stepCount > 260){
        rdyDelete_ = true;
    }
    stepCount++;
}

bool PixelFlow::Drop::getRdyDelete(){
    return rdyDelete_;
}

PixelFlow::SurfaceParticle::SurfaceParticle(Vector3i maxPos, Vector3f pos, Vector3f vel, Vector3f accel, Color col) :
        Particle(pos, vel, accel, col)
        {
    maxPosition = maxPos;
}

void PixelFlow::SurfaceParticle::step(){
    accelerateOnSurface();
    move();
    warp();
}

void PixelFlow::SurfaceParticle::accelerateOnSurface(){
    switch(getScreenNumberThis(iPosition())){
        case ScreenNumber::top:
        case ScreenNumber::bottom:
            velocity_[0] += acceleration_[0];
            velocity_[1] += acceleration_[1];
            break;
        case ScreenNumber::front:
        case ScreenNumber::back:
            velocity_[0] += acceleration_[0];
            velocity_[2] += acceleration_[2];
            break;
        case ScreenNumber::left:
        case ScreenNumber::right:
            velocity_[1] += acceleration_[1];
            velocity_[2] += acceleration_[2];
            break;
        case ScreenNumber::anyScreen:
        default:
            break;
    }
};

void PixelFlow::SurfaceParticle::warp(){
    //constrain position values
    for(int i = 0; i < 3; i++)
        position_[i] = constrain(position_[i], 0.0f, (float)VIRTUALCUBEMAXINDEX);

    Vector3i currentPosition = iPosition();
    EdgeNumber currentEdge = getEdgeNumberThis(currentPosition);

    if(currentEdge != anyEdge){
        if(currentEdge != lastEdge){
            switch (currentEdge) {
                case topLeft:
                case topRight:
                case bottomRight:
                case bottomLeft:
                    std::swap(velocity_[2] ,velocity_[0]);
                    break;
                case topFront:
                case topBack:
                case bottomBack:
                case bottomFront:
                    std::swap(velocity_[2] ,velocity_[1]);
                    break;
                case frontRight:
                case backLeft:
                case leftFront:
                case rightBack:
                    std::swap (velocity_[0],velocity_[1]);
                    break;
                case anyEdge:
                default:
                    break;
            }
            //set position to the rounded position to eliminate being always slightly below the surface due to rounding errors
            position_ = currentPosition.cast<float>();
            //constrain velocity directions, reflect if neccessary
            if((currentPosition[0] == 0 && velocity_[0] < 0) || (currentPosition[0] == VIRTUALCUBEMAXINDEX && velocity_[0] > 0)) velocity_[0] *= -1;
            if((currentPosition[1] == 0 && velocity_[1] < 0) || (currentPosition[1] == VIRTUALCUBEMAXINDEX && velocity_[1] > 0)) velocity_[1] *= -1;
            if((currentPosition[2] == 0 && velocity_[2] < 0) || (currentPosition[2] == VIRTUALCUBEMAXINDEX && velocity_[2] > 0)) velocity_[2] *= -1;
        }
    }
    lastIPosition = currentPosition;
    lastEdge = currentEdge;
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
