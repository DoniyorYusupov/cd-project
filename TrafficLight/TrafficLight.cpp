#include <iostream>
#include <string>
#include "TrafficLight.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

TrafficLight::TrafficLight() {
    detected = false;
}

bool TrafficLight::detectLight(Mat inputImage) {
    Mat mask1, mask2, mask, hsv;
    int numOfRedDots = 0;

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cvtColor(inputImage, hsv, COLOR_BGR2HSV);

    inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(160, 100, 100), Scalar(180, 255, 255), mask2);
//
//    inRange(hsv, Scalar(0, 100, 100), Scalar(30, 255, 255), mask1);
//    inRange(hsv, Scalar(150, 100, 100), Scalar(179, 255, 255), mask2);

    mask = mask1 | mask2;
    Canny(mask, canny_output, 75, 150);

    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


    for (auto contour : contours) {
        double area = contourArea(contour);
//        cout << "Area: " << area << endl;
        if (area > 20 && area < 800) {
            double arc = arcLength(contour, true);
            vector<Point> approxContours;
            approxPolyDP(contour, approxContours, 0.01 * arc, true);
            if (approxContours.size() > 5) {
                numOfRedDots++;
            }
        }
    }
    if (numOfRedDots > 0) {
        detected = true;
    } else {
        detected = false;
    }
    imshow("IMG", mask);
    return detected;
}

