#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H

#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

class MotorManager
{
private:
    int mMaxSpeed;
    int mMinSpeed;
    int mSpeed;
    int mRotateSpeed;

public:
    MotorManager(int minSpeed = 0, int maxSpeed = 100, int speed = 90, int rotateSpeed = 70);
    void goForward(int delay);
    void goBackward(int delay);
    void goRight(int delay);
    void goLeft(int delay);
    void goRightSmooth(int delay);
    void goLeftSmooth(int delay);
    void stop();
    void move(bool isLeft, bool isRight, bool redDetected, bool isStopDetected, bool isPedestrianDetected, bool isUltrasonic);
    bool inRange(unsigned low, unsigned high, unsigned x);
    ~MotorManager();
};
#endif
