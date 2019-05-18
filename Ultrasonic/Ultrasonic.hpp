//
// Created by daniel on 5/12/19.
//

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define TRIG_PIN 28
#define ECHO_PIN 29

class Ultrasonic
{
private:

public:
    Ultrasonic(void);
    void stop();
    void start();
    long getDistance();
    ~Ultrasonic();
};
#endif //ULTRASONIC_H
