#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>


#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

class TrafficLight
{
private:
    bool detected;
public:
    TrafficLight();
    bool detectLight(cv::Mat inputImage);
};
#endif
