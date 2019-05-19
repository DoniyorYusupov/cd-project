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


bool TrafficLight::detectLight(Mat inputImage) {
    Mat mask1, mask2, mask, hsv;

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cvtColor(inputImage, hsv, COLOR_BGR2HSV);

    inRange(hsv, Scalar(0, 150, 100), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(170, 150, 100), Scalar(180, 255, 255), mask2);


    bitwise_or(mask1, mask2, mask);

    findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


    for (auto contour : contours) {
        double area = contourArea(contour);
        vector<Point> approxContours;
        approxPolyDP(contour, approxContours, 0.02 * arcLength(contour, true), true);
        if (area > 130 && area < 800) {
            return true;
        }
    }

    return false;
}

