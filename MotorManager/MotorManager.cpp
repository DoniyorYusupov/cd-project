#include <softPwm.h>
#include <wiringPi.h>
#include<iostream>
#include <string>
#include <vector>
#include "MotorManager.hpp"


MotorManager::MotorManager(int minSpeed, int maxSpeed, int speed, int rotateSpeed)
{
    mMaxSpeed = maxSpeed;
    mMinSpeed = minSpeed;
    mSpeed = speed;
    mRotateSpeed = rotateSpeed;

    pinMode(IN1_PIN, SOFT_PWM_OUTPUT);
    pinMode(IN2_PIN, SOFT_PWM_OUTPUT);
    pinMode(IN3_PIN, SOFT_PWM_OUTPUT);
    pinMode(IN4_PIN, SOFT_PWM_OUTPUT);

    softPwmCreate(IN1_PIN, mMinSpeed, mMaxSpeed);
    softPwmCreate(IN2_PIN, mMinSpeed, mMaxSpeed);
    softPwmCreate(IN3_PIN, mMinSpeed, mMaxSpeed);
    softPwmCreate(IN4_PIN, mMinSpeed, mMaxSpeed);
}

MotorManager::~MotorManager()
{
    softPwmWrite(IN1_PIN, mMinSpeed);
    softPwmWrite(IN2_PIN, mMinSpeed);
    softPwmWrite(IN3_PIN, mMinSpeed);
    softPwmWrite(IN4_PIN, mMinSpeed);

    pinMode(IN1_PIN, INPUT);
    pinMode(IN2_PIN, INPUT);
    pinMode(IN3_PIN, INPUT);
    pinMode(IN4_PIN, INPUT);
}

void MotorManager::goForward(int time)
{
    softPwmWrite(IN1_PIN, mSpeed);
    softPwmWrite(IN2_PIN, mMinSpeed);
    softPwmWrite(IN3_PIN, mSpeed);
    softPwmWrite(IN4_PIN, mMinSpeed);
    delay(time);
}

void MotorManager::goBackward(int time)
{
    softPwmWrite(IN1_PIN, mMinSpeed);
    softPwmWrite(IN2_PIN, mSpeed);
    softPwmWrite(IN3_PIN, mMinSpeed);
    softPwmWrite(IN4_PIN, mSpeed);
    delay(time);
}

void MotorManager::goRight(int time)
{
    softPwmWrite(IN1_PIN, mSpeed);
    softPwmWrite(IN2_PIN, mMinSpeed);
    softPwmWrite(IN3_PIN, mMinSpeed);
    softPwmWrite(IN4_PIN, mSpeed);
    delay(time);
}

void MotorManager::goLeft(int time)
{
    softPwmWrite(IN1_PIN, mMinSpeed);
    softPwmWrite(IN2_PIN, mSpeed);
    softPwmWrite(IN3_PIN, mSpeed);
    softPwmWrite(IN4_PIN, mMinSpeed);
    delay(time);
}

void MotorManager::goRightSmooth(int time)
{
    softPwmWrite(IN1_PIN, mRotateSpeed);
    softPwmWrite(IN2_PIN, mMinSpeed);
    softPwmWrite(IN3_PIN, mMinSpeed);
    softPwmWrite(IN4_PIN, mMinSpeed);
    delay(time);
}

void MotorManager::goLeftSmooth(int time)
{
    softPwmWrite(IN1_PIN, mMinSpeed);
    softPwmWrite(IN2_PIN, mMinSpeed);
    softPwmWrite(IN3_PIN, mRotateSpeed);
    softPwmWrite(IN4_PIN, mMinSpeed);
    delay(time);
}

void MotorManager::stop()
{
    softPwmWrite(IN1_PIN, mMinSpeed);
    softPwmWrite(IN2_PIN, mMinSpeed);
    softPwmWrite(IN3_PIN, mMinSpeed);
    softPwmWrite(IN4_PIN, mMinSpeed);
}


/*void MotorManager::move(std::string turn, std::vector<double> degrees)
{
    int left = abs(degrees[0]);
    int right = abs(degrees[1]);

    if (turn == "Left Turn") {
        if (inRange(0, 10, left)) {
            goLeft(50);
        } else if (inRange(21, 40, left)) {
            goLeft(100);
        } else if (inRange(41, 200, left)) {
            goLeft(150);
        }
    } else if (turn == "Right Turn") {
        if (inRange(0, 10, right)) {
            goRight(50);
        } else if (inRange(21, 40, right)) {
            goRight(100);
        } else if (inRange(41, 200, right)) {
            goRight(150);
        }
    } else {
        goForward(50);
    }

}*/

void MotorManager::move(bool isLeft, bool isRight, bool redDetected, bool isStopDetected, bool isPedestrianDetected, bool isUltrasonic)
{
//    if (isStopDetected) {
//        stop();
//        delay(5000);
//        goRight(100);
//    }
//    if (isPedestrianDetected) {
//        mSpeed = mSpeed / 2;
//        goForward(100);
//        goRight(100);
//        mSpeed = mSpeed * 2;
//    }
    if (redDetected || isUltrasonic) {
//        std::cout << "stop" << std::endl;
        stop();
    } else if (isLeft && isRight) {
        goForward(100);
    }
    else if (isLeft) {
//        std::cout << "go right" << std::endl;
        goRight(180);
    } else if (isRight) {
//        std::cout << "go left" << std::endl;
        goLeft(180);
    } else {
        goForward(20);
    }


}


bool MotorManager::inRange(unsigned low, unsigned high, unsigned x)
{
    return (low <= x && x <= high);
}