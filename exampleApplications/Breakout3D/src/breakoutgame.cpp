#include "breakoutgame.h"
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <cmath>


#include <iostream>
#include <fstream>

BreakoutGame::BreakoutGame() : CubeApplication(40){
  reset();
  updateHighScoreFromToFile();
}

void BreakoutGame::reset(int gameDuration){
  gameState_ = pregame;
  remainingSeconds_ = gameDuration;
  players_.clear();
  balls_.clear();
  blocks_.clear();
  joysticks_.clear();
  joysticks_.push_back(new Joystick(0));
  joysticks_.push_back(new Joystick(1));
  players_.push_back(new Player(this, 0, joysticks_[0]));
  players_.push_back(new Player(this, 1, joysticks_[1]));
  spawnBallForPlayer(0);
  spawnBallForPlayer(1);
  int blockSize = 4;
  int blockScore = 25;
  for(int i = 0; i<CUBESIZE; i+=blockSize)
    for(int j = 0; j<CUBESIZE; j+=blockSize)
      blocks_.push_back(new Block(this, top, Vector2i(i,j), blockSize, blockScore, (Color::randomGreen() + Color::blue()*0.5 + Color::randomBlue())*0.7));
}

void BreakoutGame::spawnBallForPlayer(int playerId){
  switch (playerId) {
    case 0:
      balls_.push_back(new Ball(this, Vector3f(VIRTUALCUBECENTER, 0, VIRTUALCUBECENTER), Vector3f(0, 0, 1), 1.7));
    break;
    case 1:
      balls_.push_back(new Ball(this, Vector3f(VIRTUALCUBECENTER, VIRTUALCUBEMAXINDEX, VIRTUALCUBECENTER), Vector3f(0, 0, 1), 1.7));
    break;
  }
}

BreakoutGame::Player * BreakoutGame::getLeadingPlayer(){
  Player * leadingPlayer = NULL;
  int leadScore = 0;
  for(auto player : players_){
    if(player->score() >= leadScore){
      leadScore = player->score();
      leadingPlayer = player;
    }
  }
  return leadingPlayer;
}

void BreakoutGame::ballLoop(){
  for(auto ball : balls_){
    ball->step();
    ball->render();
    //Ball dying
    if(ball->iPosition()[2] == VIRTUALCUBEMAXINDEX && !ball->isDead()){
//      soundPlayer_.playGameSound(2);
      if(ball->lastPlayer() != NULL){
        ball->lastPlayer()->addToScore(-10);
      }
      ball->die();
    }
  }
  // balls_.erase(std::remove_if(balls_.begin(),balls_.end(),[](Ball * b){return (b->isDead());}),balls_.end());
}

void BreakoutGame::blockLoop(){
  for(auto block : blocks_){
    block->render();
    for(auto ball : balls_){
      if(block->collidesWith(ball->position())) {
//        soundPlayer_.playGameSound(1);
        //add score to the last player who touched the ball
        if(ball->lastPlayer() != NULL)
          ball->lastPlayer()->addToScore(block->score());

        //Do the collision reflection
        Vector3f incidentVec3 = ball->position() - block->centerPosition();
        Vector3f reflectionVector;
        std::vector<Vector3f> possibleReflectionVectors;
        float angle = atan2(incidentVec3[0], incidentVec3[1]) * 180 / M_PI;
        while(angle < 0)
          angle += 360; //make the angle value positiv

        //first step: figure out some possible reflection vectors
        int angleTolerance = 10;
        if((angle > 315 - angleTolerance || angle < 45 + angleTolerance)){
          possibleReflectionVectors.push_back(Vector3f(0,1,0));
        }
        if(angle > 45 - angleTolerance && angle < 135 + angleTolerance){
          possibleReflectionVectors.push_back(Vector3f(1,0,0));
        }
        if(angle > 135 - angleTolerance && angle < 225 + angleTolerance){
          possibleReflectionVectors.push_back(Vector3f(0,-1,0));
        }
        if(angle > 225 - angleTolerance && angle < 315 + angleTolerance){
          possibleReflectionVectors.push_back(Vector3f(-1,0,0));
        }
        // second step: iterate through possibleReflectionVectors and check if there is a neighbour block disabling this possible reflection vector
        for(auto vect : possibleReflectionVectors){
          if(!isBlockAtPoint(block->centerPosition() + vect * block->size())){
            reflectionVector = vect;
          }
        }
        ball->reflect(reflectionVector);
        block->die();
      }
    }
  }
  blocks_.erase(std::remove_if(blocks_.begin(),blocks_.end(),[](Block * b){return (b->isDead());}),blocks_.end());
}

void BreakoutGame::playerLoop(){
  for(auto player : players_){
    player->step();
    player->render();
    for(auto ball : balls_){
      if(player->collidesWith(ball->iPosition())) {
//        soundPlayer_.playGameSound(0);
        Vector3f collisionVector = ball->iPosition().cast<float>() - player->centerPosition().cast<float>(); //Vector3f(0,0,-8)
        collisionVector[2] = -15; //to always be upward facing
        switch(getScreenNumber(ball->iPosition())){
          case front:
          case back:
            collisionVector[1] = 0;
          break;
          case right:
          case left:
            collisionVector[0] = 0;
          break;
          default:
          break;
        }
        ball->reflect(collisionVector);
        Vector3f bvel = ball->velocity();
        if(bvel[2] > 0) //make sure to reflect upwards
          bvel[2] *= -1;
        if(bvel[2] > -0.5) //constrain flat angles
          bvel[2] = -0.5;
        ball->velocity(bvel);
        ball->resetSpeed();
        player->blink(Color::white());

        //set last player to the ball and vice versa
        ball->setLastPlayer(player);
        player->setLastBall(ball);
      }
    }
  }
}

bool BreakoutGame::loop(){
  static long loopcount = 1;
  static int postgameCounter = 0;
  static int scrNrCounter = 0;
  static bool isHighScore = false;

  switch(gameState_){
    case pregame:
    {
      clear();
      //show the blocks
      blockLoop();
      std::string playtext = "PRESS A TO PLAY";
      std::string rbutton = "R:  SLOMO";
      std::string bbutton = "B: ROCKET";
      for(int i = 0; i < 4; i++){
        drawText((ScreenNumber)i, Vector2i(CharacterBitmaps::centered, 20), Color::white(), playtext);
        drawText((ScreenNumber)i, Vector2i(CharacterBitmaps::centered, 30), Color::white()*0.5, rbutton);
        drawText((ScreenNumber)i, Vector2i(CharacterBitmaps::centered, 36), Color::white()*0.5, bbutton);
        drawText((ScreenNumber)i, Vector2i(CharacterBitmaps::right, 58), Color::white()*0.5, std::to_string(currentHighScore));
      }
      for(auto joystick : joysticks_){
        if(joystick->getButtonPress(0)){
          std::cout << "start game!" << std::endl;
          gameState_ = ingame;
          //init ball respawn for nice countdown
          for(auto ball : balls_){
            ball->die();
            ball->position(Vector3f(CUBECENTER,CUBECENTER,CUBECENTER)); //hide the balls
          }
        }
        joystick->clearAllButtonPresses();
      }
      break;
    }
    case ingame:
      clear();
      ballLoop();
      blockLoop();
      playerLoop();

      //render balls at last to always be on top
      for(auto ball : balls_)
        ball->render();

      scrNrCounter = 0;
      for(auto player : players_){
        drawText((ScreenNumber)scrNrCounter, Vector2i(0,58), player->color(), std::to_string(player->getId()) + ": ");
        drawText((ScreenNumber)scrNrCounter, Vector2i(8,58), Color::white(), std::to_string(player->score()));
        drawText((ScreenNumber)(scrNrCounter+2), Vector2i(0,58), player->color(), std::to_string(player->getId()) + ": ");
        drawText((ScreenNumber)(scrNrCounter+2), Vector2i(8,58), Color::white(), std::to_string(player->score()));
        scrNrCounter++;
      }

      drawText(front, Vector2i(CharacterBitmaps::right, 58), Color::white(), std::to_string(remainingSeconds_));
      drawText(back, Vector2i(CharacterBitmaps::right, 58), Color::white(), std::to_string(remainingSeconds_));

      if(loopcount%getFps() == 0)
        remainingSeconds_--;

      //end of game
      if(remainingSeconds_ < 0){
        gameState_ = postgame;
        postgameCounter = 10;
      }

      //reset game
      for(auto joystick : joysticks_){
        if(joystick->getButtonPress(10)){
          std::cout << "reset game!" << std::endl;
          reset();
        }
      }
    break;
    case postgame:
      clear();
      scrNrCounter = 0;
      for(auto p : players_){
        drawText((ScreenNumber)scrNrCounter, Vector2i(CUBECENTER-24,CharacterBitmaps::centered), p->color(), "PLAYER "+ std::to_string(p->getId()) + ": ");
        drawText((ScreenNumber)scrNrCounter, Vector2i(CUBECENTER+13,CharacterBitmaps::centered), Color::white(), std::to_string(p->score()));
        drawText((ScreenNumber)(scrNrCounter+2), Vector2i(CUBECENTER-24,CharacterBitmaps::centered), p->color(), "PLAYER "+ std::to_string(p->getId()) + ": ");
        drawText((ScreenNumber)(scrNrCounter+2), Vector2i(CUBECENTER+13,CharacterBitmaps::centered), Color::white(), std::to_string(p->score()));
        scrNrCounter++;
      }
      if(postgameCounter%2){
        std::string topstring = "PLAYER " + std::to_string(getLeadingPlayer()->getId()) + " WON";
        drawText(top, Vector2i(CharacterBitmaps::centered,CharacterBitmaps::centered), getLeadingPlayer()->color(), topstring);
      }
      if(loopcount/2%2 == 0){
          if (updateHighScoreFromToFile(getLeadingPlayer()->score())) {
              isHighScore = true;
          }
          if(isHighScore)
              drawText(top, Vector2i(CharacterBitmaps::centered,20), Color::white(), "NEW HIGHSCORE");
      }
      if(loopcount%(getFps()/4) == 0)
        postgameCounter--;
      if(postgameCounter < 0){
          reset();
          isHighScore = false;
      }
    break;
  }

  // if(loopcount % getFps() == 0)
  //   std::cout << "load: " << getLoad() << std::endl;
  render();
  loopcount++;
  return true;
}

bool BreakoutGame::updateHighScoreFromToFile(int score, std::string filename) {
    bool returnValue = false;
    std::ifstream configFileReadStream(filename);

    std::fstream highScoreFile;
    highScoreFile.open(filename.data(), std::fstream::binary | std::fstream::in);
    if (highScoreFile) {
        highScoreFile >> currentHighScore;
        std::cout << "file open successful, highscore: " << currentHighScore << std::endl;
    } else {
        //create file
        highScoreFile.open(filename.data(),
                           std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
        highScoreFile << 0;
        std::cout << "file created successful" << std::endl;
    }

    if (score > currentHighScore) {
        std::cout << "NEW HIGHSCORE: " << score << std::endl;
        currentHighScore = score;
        returnValue = true;

        highScoreFile.close();
        highScoreFile.open(filename.data(),
                           std::fstream::binary | std::fstream::in | std::fstream::out | std::fstream::trunc);
        highScoreFile << currentHighScore;
    }

    highScoreFile.close();
    return returnValue;
}

bool BreakoutGame::isBlockAtPoint(Vector3f point){
  bool result = false;
  for(auto block : blocks_){
    if(block->collidesWith(point))
      result = true;
  }
  return result;
}

BreakoutGame::Player::Player(CubeApplication * renderCube, int id, Joystick * joystick){
  score_ = 0;
  id_ = id;
  ca_ = renderCube;
  width_ = 15;
  height_ = 3;
  if(id == 0){
    color_ = Color::green();
    pos_ = VIRTUALCUBECENTER;
    minPos_ = 0 + width_/2;
    maxPos_ = 128 - width_/2;
  }else if(id == 1){
    color_ = Color::red();
    pos_ = VIRTUALCUBECENTER + 128;
    minPos_ = 128 + width_/2;
    maxPos_ = 256 - width_/2;
  }
  blinkColor_ = color_;
  blinkCount_ = 0;
  joystick_ = joystick;
  lastBall_ = NULL;
  vel_ = 0;
}

void BreakoutGame::Player::step(){
  if(joystick_->isFound()){
    vel_ = joystick_->getAxis(0) * 1.8;
    //do powerups and stuff
    if(lastBall_ != NULL){
      if(joystick_->getButton(7) && score_ > 0){
        lastBall_->setSpeed(0.5);
        addToScore(-0.025);
      }else{
        lastBall_->resetSpeed();
      }
      if(joystick_->getButtonPress(1)
          && score_ > 100
          && ca_->getScreenNumber(lastBall_->iPosition()) != top
          && !ca_->isOnEdge(lastBall_->iPosition())
          && (lastBall_->velocity()[0] != 0 || lastBall_->velocity()[1] != 0 || lastBall_->velocity()[2] > 0)){
        lastBall_->velocity(Vector3f(0,0,-1));
        lastBall_->resetSpeed();
        addToScore(-100);
      }
    }
  }else{
    doKIMove();
  }
  pos_ += vel_;
  pos_ = constrain(pos_, (float)minPos_, (float)maxPos_);
  generatePaddle();
}

void BreakoutGame::Player::doKIMove(){
  if(lastBall_ != NULL){
    Vector3i BallPos = lastBall_->iPosition();
    int xPosBall = 0;
    if(ca_->getScreenNumber(BallPos) == front)
      xPosBall = BallPos[0];
    else if(ca_->getScreenNumber(BallPos) == right)
      xPosBall = BallPos[1] + CUBESIZE;
    else if(ca_->getScreenNumber(BallPos) == back)
      xPosBall = CUBESIZE - BallPos[0] + 128;
    else if(ca_->getScreenNumber(BallPos) == left)
      xPosBall = CUBESIZE - BallPos[1] + CUBESIZE + 128;
    //randomize xPosBall
    xPosBall += 10-rand()%20;
    if(xPosBall < pos_)
      vel_ = -1;
    else if(xPosBall > pos_)
      vel_ = 1;
    else
      vel_ = 0;
    // vel_ += 0.125-((rand()%100)/400);
    // std::cout << "KI Move: xposball: " << xPosBall << " pos: " << pos_ << " vel: " << vel_ << std::endl;
  }else{
    if(id_ == 0)
      pos_ = VIRTUALCUBECENTER;
    else if(id_ == 1)
      pos_ = VIRTUALCUBECENTER + 128;
  }
}

void BreakoutGame::Player::generatePaddle(){
  paddlePixels_.clear();
  int intPos = round(pos_);
  for(int i = 8; i < 8+height_; i++){
    for(int j = intPos-(width_/2); j < intPos+(width_/2); j++){
      Vector3i tempPoint;
      int k = j%256;
      while(k < 0)
        k += 256;
      if(k < 64){
        tempPoint = ca_->getPointOnScreen(front, Vector2i(k,CUBEMAXINDEX-i));
      }else if(k < 128){
        tempPoint = ca_->getPointOnScreen(right, Vector2i(k-64,CUBEMAXINDEX-i));
      }else if(k < 192){
        tempPoint = ca_->getPointOnScreen(back, Vector2i(k-128,CUBEMAXINDEX-i));
      }else if(k < 256){
        tempPoint = ca_->getPointOnScreen(left, Vector2i(k-192,CUBEMAXINDEX-i));
      }
      paddlePixels_.push_back(tempPoint);
      if(j == intPos)
        centerPosition_ = tempPoint;
      //corners:
      if(k == 64){
        paddlePixels_.push_back(Vector3i(VIRTUALCUBEMAXINDEX,0,CUBEMAXINDEX-i));
      }
      if(k == 192){
        paddlePixels_.push_back(Vector3i(0,VIRTUALCUBEMAXINDEX,CUBEMAXINDEX-i));
      }
    }
  }
}

void BreakoutGame::Player::setLastBall(Ball * ball){
  lastBall_ = ball;
}

BreakoutGame::Ball * BreakoutGame::Player::lastBall(){
  return lastBall_;
}

bool BreakoutGame::Player::collidesWith(Vector3i pos){
  for(auto p : paddlePixels_){
    if(p == pos)
      return true;
  }
  return false;
}

void BreakoutGame::Player::render(){
  Color paddleColor = color_;
  if(blinkCount_-- > 0)
    paddleColor = blinkColor_;
  for(auto p : paddlePixels_)
    ca_->setPixel3D(p, paddleColor);
}

Vector3i BreakoutGame::Player::centerPosition(){
  // return Vector3i(round(position_[0]),round(position_[1]),round(position_[2]));
  return centerPosition_;
}

void BreakoutGame::Player::blink(Color color){
  blinkCount_ = 2;
  blinkColor_ = color;
}

bool BreakoutGame::Player::addToScore(float value){
  score_ += value;
  if(score_ < 0){
    score_ = 0;
    return false;
  }
  return true;
}

int BreakoutGame::Player::score(){
  return (int)score_;
}

int BreakoutGame::Player::getId(){
  return id_;
}

Color BreakoutGame::Player::color(){
  return color_;
}

Joystick * BreakoutGame::Player::joystick(){
  return joystick_;
}

BreakoutGame::Ball::Ball(CubeApplication * renderCube, Vector3f startPosition, Vector3f startVelocity, float speed)
:position_(startPosition),
 velocity_(startVelocity.normalized()*speed),
 acceleration_(0,0,0),
 color_(Color::white()){
    ca_ = renderCube;
    isDead_ = false;
    defaultPosition_ = startPosition;
    defaultVelocity_ = startVelocity.normalized()*speed;
    lastPlayer_ = NULL;
    speed_ = speed;
    defaultSpeed_ = speed_;
    lastEdge_ = anyEdge;
    lastIPosition_ = iPosition();
}

void BreakoutGame::Ball::setSpeed(float speed){
  speed_ = speed;
  velocity_ = velocity_.normalized()*speed_;
}

void BreakoutGame::Ball::resetSpeed(){
  setSpeed(defaultSpeed_);
}

void BreakoutGame::Ball::step(){
  if(!isDead_){
    accelerate();
    move();
    //constrain position values
    for(int i = 0; i < 3; i++)
      position_[i] = constrain(position_[i], 0.0f, (float)VIRTUALCUBEMAXINDEX);

    Vector3i currentPosition = iPosition();
    EdgeNumber currentEdge = ca_->getEdgeNumber(currentPosition);

    if(currentEdge != anyEdge ){
      if(currentEdge != lastEdge_){
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
            std::swap (velocity_[0],velocity_[1]);
          break;
          //to disable wrapping around these edges, the next two edges are identical to the the default case -> reflection
          case leftFront:
          case rightBack:
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

    if(!ca_->isOnSurface(currentPosition)) std::cout << "[WARN] Ball not on Surface: " << currentPosition[0] << ", " << currentPosition[1] << ", " << currentPosition[2] << std::endl;
    lastIPosition_ = currentPosition;
    lastEdge_ = currentEdge;
  }else{ //isDead == TRUE
    //red line at the bottom as dying animation
    if(respawnTimer_ % 2 == 0 && respawnTimer_ > ca_->getFps()){
      switch (ca_->getEdgeNumber(iPosition())) {
        case bottomFront:
          ca_->drawLine3D(Vector3i(0,0,CUBESIZE),Vector3i(CUBESIZE,0,CUBESIZE), Color::red());
        break;
        case bottomRight:
          ca_->drawLine3D(Vector3i(VIRTUALCUBEMAXINDEX,0,CUBESIZE),Vector3i(VIRTUALCUBEMAXINDEX,CUBESIZE,CUBESIZE), Color::red());
        break;
        case bottomLeft:
          ca_->drawLine3D(Vector3i(0,0,CUBESIZE),Vector3i(0,CUBESIZE,CUBESIZE), Color::red());
        break;
        case bottomBack:
          ca_->drawLine3D(Vector3i(0,VIRTUALCUBEMAXINDEX,CUBESIZE),Vector3i(CUBESIZE,VIRTUALCUBEMAXINDEX,CUBESIZE), Color::red());
        break;
        default:
        break;
      }
    }

    if(respawnTimer_-- <= 0)
      reset();

    // countdown timer for ball respawn
    ca_->drawText(ca_->getScreenNumber(defaultPosition_.cast<int>()), Vector2i(CharacterBitmaps::centered, CharacterBitmaps::centered), Color::white(), std::to_string((int)(respawnTimer_/ca_->getFps())+1));
  }
}


void BreakoutGame::Ball::move(){
  position_ += velocity_;
}

void BreakoutGame::Ball::accelerate(){
  velocity_ += acceleration_;
}

void BreakoutGame::Ball::revertStep(){
  position_ -= velocity_;
  velocity_ -= acceleration_;
}

Vector3f BreakoutGame::Ball::position(){
  return position_;
}

Vector3f BreakoutGame::Ball::velocity(){
  return velocity_;
}

Vector3f BreakoutGame::Ball::acceleration(){
  return acceleration_;
}

Vector3i BreakoutGame::Ball::iPosition(){
  return Vector3i(round(position()[0]),round(position()[1]),round(position()[2]));
}

void BreakoutGame::Ball::position(Vector3f pos){
  position_ = pos;
}
void BreakoutGame::Ball::velocity(Vector3f vel){
  velocity_ = vel;
}
void BreakoutGame::Ball::acceleration(Vector3f accel){
  acceleration_ = accel;
}

Color BreakoutGame::Ball::color(){
  return color_;
}
void BreakoutGame::Ball::color(Color Col){
  color_ = Col;
}

void BreakoutGame::Ball::render(){
  ca_->setPixelSmooth3D(position(),color_);
}

void BreakoutGame::Ball::reflect(Vector3f reflectionVector){
  revertStep();
  //d−2(d⋅n)n
  velocity_ = velocity_ - 2 * (velocity_.dot(reflectionVector.normalized())) * reflectionVector.normalized();
  step();
}

void BreakoutGame::Ball::die(){
  isDead_ = true;
  respawnTimer_ = ca_->getFps()*2; // 2 seconds
}

void BreakoutGame::Ball::reset(){
  isDead_ = false;
  position_ = defaultPosition_;
  velocity_ = defaultVelocity_;
  lastPlayer_ = NULL;
  lastEdge_ = anyEdge;
  lastIPosition_ = iPosition();
}

bool BreakoutGame::Ball::isDead(){
  return isDead_;
}

void BreakoutGame::Ball::setLastPlayer(Player * player){
  lastPlayer_ = player;
}

BreakoutGame::Player * BreakoutGame::Ball::lastPlayer(){
  return lastPlayer_;
}

BreakoutGame::Block::Block(CubeApplication * renderCube, ScreenNumber screenNr, Vector2i topLeftCorner, int size, int score, Color color){
  ca_ = renderCube;
  color_ = color;
  isDead_ = false;
  topLeftCorner_ = topLeftCorner;
  size_ = size;
  screenNr_ = screenNr;
  score_ = score;
  for(int i = topLeftCorner[0]; i < topLeftCorner[0] + size_; i++){
    for(int j = topLeftCorner[1]; j < topLeftCorner[1] + size_; j++){
      blockPixels_.push_back(ca_->getPointOnScreen(screenNr_, Vector2i(i,j)));
    }
  }
}

void BreakoutGame::Block::render(){
  if(!isDead_){
    for(auto p : blockPixels_)
      ca_->setPixel3D(p, color_);
  }
}

bool BreakoutGame::Block::collidesWith(Vector3f pos){
  if(isDead_)
    return false;
  for(auto p : blockPixels_){
    if(p == pos.cast<int>())
      return true;
  }
  return false;
}

void BreakoutGame::Block::die(){
  isDead_ = true;
}

bool BreakoutGame::Block::isDead(){
  return isDead_;
}

int BreakoutGame::Block::score(){
  return score_;
}

int BreakoutGame::Block::size(){
  return size_;
}

Vector3f BreakoutGame::Block::centerPosition(){
  return ca_->getPointOnScreen(screenNr_, Vector2f(topLeftCorner_.cast<float>() + Vector2f((float)size_/2 - 0.5f,(float)size_/2 - 0.5f)));
}
