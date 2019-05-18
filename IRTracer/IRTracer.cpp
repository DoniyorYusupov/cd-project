#include <wiringPi.h>
#include <softPwm.h>
#include "IRTracer.hpp"

IRTracer::IRTracer() {
    start();
}

IRTracer::~IRTracer() {
    stop();
}

void IRTracer::start() {
    pinMode(LEFT_PIN, INPUT);
    pinMode(RIGHT_PIN, INPUT);
}

void IRTracer::stop() {
    pinMode(LEFT_PIN, OUTPUT);
    pinMode(RIGHT_PIN, OUTPUT);
}

long IRTracer::isLeft() {
    return digitalRead(LEFT_PIN) == 0;
}

long IRTracer::isRight() {
    return digitalRead(RIGHT_PIN) == 0;
}