#include <wiringPi.h>
#include <softPwm.h>
#include "Ultrasonic.hpp"

Ultrasonic::Ultrasonic(void){
    start();
}

Ultrasonic::~Ultrasonic()
{
    stop();
}

void Ultrasonic::stop(){
    pinMode(TRIG_PIN, INPUT);
    pinMode(ECHO_PIN, OUTPUT);
}

void Ultrasonic::start(){
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

long Ultrasonic::getDistance() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIG_PIN, LOW);

    while(digitalRead(ECHO_PIN) == LOW) {}
    long start = micros();
    while(digitalRead(ECHO_PIN) == HIGH) {}
    long end = micros();

    long distance = (end - start) / 29. / 2;

    return distance;
}
