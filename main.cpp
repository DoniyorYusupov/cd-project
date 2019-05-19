#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <raspicam/raspicam_cv.h>
#include "wiringPi.h"
#include "opencv2/imgproc.hpp"
//#include <opencv2/objdetect.hpp>
#include "LaneDetector/LaneDetector.hpp"
#include "MotorManager/MotorManager.hpp"
#include "IRTracer/IRTracer.hpp"
#include "TrafficLight/TrafficLight.hpp"
#include "Ultrasonic/Ultrasonic.hpp"

using namespace std;
using namespace cv;


int main() {
    if (wiringPiSetup() == -1) {
        return 0;
    }

    Mat src;
    bool redDetected = false;
    raspicam::RaspiCam_Cv capture;
//    CascadeClassifier cascade_stop, cascade_pedestrian;
    LaneDetector laneDetector;

    Ultrasonic ultrasonic = Ultrasonic();
//    MotorManager motorManager = MotorManager(0, 80, 50, 55);
    MotorManager motorManager = MotorManager(0, 70, 45, 50);
    TrafficLight trafficLight;
    IRTracer irTracer = IRTracer();
    capture.open();
//    cascade_stop.load("stop-cascade.xml");
//    cascade_pedestrian.load("pedestrian.xml");
//    vector<Rect> stop;
//    vector<Rect> pedestrian;


    while(1) {
        capture.grab();
        capture.retrieve(src);
        resize(src, src, Size(320, 240));

        Mat red_cropped = src(Rect(0, 40, 320, 70));
        Mat red_noise = laneDetector.deNoise(red_cropped);

        redDetected = trafficLight.detectLight(red_noise);


        bool isLeft = irTracer.isLeft();
        bool isRight = irTracer.isRight();
        long dist = ultrasonic.getDistance();
        bool isUltrasonic = dist < 15;
        bool isStopDetected = false;
        bool isPedestrianDetected = false;

//        cascade_stop.detectMultiScale(src, stop, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(5, 5));
//        cascade_pedestrian.detectMultiScale(src, pedestrian, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(5, 5));
//        if(stop.size() > 0) {
//            isStopDetected = true;
//            cout << "STOP SIGN" << endl;
//        }
//        if(pedestrian.size() > 0) {
//            isPedestrianDetected = true;
//            cout << "PEDESTRIAN SIGN" << endl;
//        }


        motorManager.move(isLeft, isRight, redDetected, isStopDetected, isPedestrianDetected, isUltrasonic);


        waitKey(1);
    }

    capture.release();
    return 0;
}

/*  g++ MotorManager/MotorManager.cpp LaneDetector/LaneDetector.cpp main.cpp -o main -I/usr/local/include -L/usr/local/lib -L/opt/vc/lib -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util `pkg-config --cflags --libs opencv` -lwiringPi -Wall  */