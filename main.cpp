#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <raspicam/raspicam_cv.h>
#include "wiringPi.h"
#include "opencv2/imgproc.hpp"
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
    bool redDetected;
    string turn;
    raspicam::RaspiCam_Cv capture;
    LaneDetector laneDetector;
    IRTracer irTracer = IRTracer();
    Ultrasonic ultrasonic = Ultrasonic();
    MotorManager motorManager = MotorManager(0, 80, 55, 60);
    TrafficLight trafficLight;
    capture.open();

    while(1) {
        capture.grab();
        capture.retrieve(src);
        resize(src, src, Size(320, 240));

        Mat light_cropped = src(Rect(0, 40, 320, 70));
        Mat light_noise = laneDetector.deNoise(light_cropped);

        redDetected = trafficLight.detectLight(light_noise);
//        bool redDetected = trafficLight.isRedDetected();

//        Mat img_cropped = src(Rect(0, 150, 320, 90));
//
//
//        Mat noise = laneDetector.deNoise(img_cropped);
//        Mat edge = laneDetector.edgeDetector(noise);
//        Mat roi = laneDetector.mask(edge);
//
//        vector<cv::Vec4i> hlines = laneDetector.houghLines(edge);
//
//        if (!hlines.empty()) {
//            vector <vector<cv::Vec4i>> lineSep = laneDetector.lineSeparation(hlines, edge);
//
//            vector <cv::Point> regress = laneDetector.regression(lineSep);
//
//            turn = laneDetector.predictTurn();
//            int flag_plot = laneDetector.plotLane(src, regress, turn);
//        }
        bool isLeft = irTracer.isLeft();
        bool isRight = irTracer.isRight();
        long dist = ultrasonic.getDistance();
        bool isUltrasonic = dist < 15;
        motorManager.move(isLeft, isRight, redDetected, isUltrasonic);


        waitKey(1);
    }

    capture.release();
    return 0;
}

/*  g++ MotorManager/MotorManager.cpp LaneDetector/LaneDetector.cpp main.cpp -o main -I/usr/local/include -L/usr/local/lib -L/opt/vc/lib -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util `pkg-config --cflags --libs opencv` -lwiringPi -Wall  */