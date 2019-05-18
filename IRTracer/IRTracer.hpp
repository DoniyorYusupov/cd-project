//
// Created by daniel on 5/12/19.
//

#ifndef IRTRACER_H
#define IRTRACER_H

#define LEFT_PIN 10
#define RIGHT_PIN 11

class IRTracer
{
private:

public:
    IRTracer(void);
    void stop();
    void start();
    long isLeft();
    long isRight();
    ~IRTracer();
};

#endif //IRTRACER_H
