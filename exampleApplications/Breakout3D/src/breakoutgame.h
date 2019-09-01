#ifndef __BREAKOUTGAME_H__
#define __BREAKOUTGAME_H__

#include <CubeApplication.h>

#include <Joystick.h>
//#include "aplay.h"

#define DEFAULTGAMEDURATION 120
#define DEFAULTHIGHSCOREFILE "/home/pi/.breakouthighscore"

class BreakoutGame : public CubeApplication {
protected:
    class Player;

    class Ball;

    class Block;

    enum GameState {
        pregame, ingame, postgame
    };
public:
    BreakoutGame();

    void playerLoop();

    void ballLoop();

    void blockLoop();

    bool loop();

    bool isBlockAtPoint(Vector3f point);

    void spawnBallForPlayer(int playerId);

    void reset(int gameDuration = DEFAULTGAMEDURATION);

    Player *getLeadingPlayer();

private:
    bool updateHighScoreFromToFile(int score = 0, std::string filename = DEFAULTHIGHSCOREFILE);

    std::vector<Player *> players_;
    std::vector<Ball *> balls_;
    std::vector<Block *> blocks_;
    std::vector<Joystick *> joysticks_;
    int remainingSeconds_;
    GameState gameState_;
//  Aplay soundPlayer_;
    int currentHighScore;
};

class BreakoutGame::Player {
public:
    Player(CubeApplication *renderCube, int id, Joystick *joystick);

    void render();

    void step();

    void doKIMove();

    void generatePaddle();

    Vector3i centerPosition();

    bool collidesWith(Vector3i pos);

    void blink(Color color);

    int getId();

    Color color();

    bool addToScore(float value);

    int score();

    void setLastBall(Ball *ball);

    Ball *lastBall();

    Joystick *joystick();

private:
    Vector3i centerPosition_;
    std::vector<Vector3i> paddlePixels_;
    int score_;
    int id_;
    int width_;
    int height_;
    float vel_;
    float pos_;
    int maxPos_;
    int minPos_;
    int blinkCount_;
    Color blinkColor_;
    Color color_;
    CubeApplication *ca_;
    Joystick *joystick_;
    Ball *lastBall_;
};

class BreakoutGame::Ball {
public:
    Ball(CubeApplication *renderCube, Vector3f startPosition, Vector3f startVelocity, float speed);

    void render();

    void reflect(Vector3f reflectionVector);

    void step();

    void accelerate();

    void move();

    void revertStep();

    void die();

    bool isDead();

    void reset();

    void setSpeed(float speed);

    void resetSpeed();

    void setLastPlayer(Player *player);

    Player *lastPlayer();

    Vector3f position();

    Vector3f velocity();

    Vector3f acceleration();

    Vector3i iPosition();

    void position(Vector3f pos);

    void velocity(Vector3f vel);

    void acceleration(Vector3f accel);

    Color color();

    void color(Color Col);

private:
    Vector3f position_;
    Vector3f velocity_;
    Vector3f acceleration_;
    Vector3f defaultPosition_;
    Vector3f defaultVelocity_;
    Color color_;
    bool isDead_;
    int respawnTimer_;
    float speed_;
    float defaultSpeed_;
    EdgeNumber lastEdge_;
    Vector3i lastIPosition_;
    CubeApplication *ca_;
    Player *lastPlayer_;
};

class BreakoutGame::Block {
public:
    Block(CubeApplication *renderCube, ScreenNumber screenNr, Vector2i topLeftCorner, int size, int score, Color color);

    void render();

    bool collidesWith(Vector3f pos);

    Vector3f centerPosition();

    void die();

    bool isDead();

    int score();

    int size();

private:
    std::vector<Vector3i> blockPixels_;
    Vector2i topLeftCorner_;
    ScreenNumber screenNr_;
    int size_;
    int score_;
    Color color_;
    CubeApplication *ca_;
    bool isDead_;
};

#endif
