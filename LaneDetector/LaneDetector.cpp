#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include "LaneDetector.hpp"

// Apply Gaussian blurring to the input Image
cv::Mat LaneDetector::deNoise(cv::Mat inputImage) {
    cv::Mat output;

    cv::GaussianBlur(inputImage, output, cv::Size(5, 5), 0);

    return output;
}

// Filter the image & apply Canny Edge to obtain only edges
cv::Mat LaneDetector::edgeDetector(cv::Mat img_noise) {
    cv::Mat output, gray_scale;
    cv::cvtColor(img_noise, gray_scale, cv::COLOR_RGB2GRAY);
    Canny(gray_scale, output, 75, 150);
    return output;
}

// Mask the edges image to only care about ROI
cv::Mat LaneDetector::mask(cv::Mat img_edges) {
    cv::Mat output;

    cv::Mat mask = cv::Mat::zeros(img_edges.size(), img_edges.type());
    img_height = img_edges.size().height;
    img_width = img_edges.size().width;

    cv::Point pts[6] = {
            cv::Point(0, img_height),
            cv::Point(0, img_height / 1.4),
            cv::Point(img_width / 2 - 50, 30),
            cv::Point(img_width / 2 + 50, 30),
            cv::Point(img_width, img_height / 1.4),
            cv::Point(img_width, img_height)
    };

    cv::fillConvexPoly(mask, pts, 6, cv::Scalar(255, 0, 0));

    // Multiply the edges image and the mask to get the output
    cv::bitwise_and(img_edges, mask, output);

    return output;
}

std::vector<cv::Vec4i> LaneDetector::houghLines(cv::Mat img_mask) {
    std::vector<cv::Vec4i> lines;

    // rho and theta are selected1 by trial and error
    HoughLinesP(img_mask, lines, 1, CV_PI / 180, 30, 30, 12);

    return lines;
}

// Separate detected lines by their slope into right and left lines
std::vector<std::vector<cv::Vec4i> > LaneDetector::lineSeparation(std::vector<cv::Vec4i> lines, cv::Mat edge) {
    std::vector<std::vector<cv::Vec4i> > output(2);
    size_t j = 0;
    cv::Point ini;
    cv::Point fini;
    double slope_thresh = 0.3; // TODO: for horizontal
    std::vector<double> slopes;
    std::vector<cv::Vec4i> selected_lines;
    std::vector<cv::Vec4i> right_lines, left_lines;


    for (auto i : lines) {
        ini = cv::Point(i[0], i[1]);
        fini = cv::Point(i[2], i[3]);

        double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y))/(static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);

        if (std::abs(slope) > slope_thresh) {
            slopes.push_back(slope);
            selected_lines.push_back(i);
        }
    }

    // Split the lines into right and left lines
    img_center =static_cast<double>((edge.cols / 2));
    while (j < selected_lines.size()) {
        ini = cv::Point(selected_lines[j][0], selected_lines[j][1]);
        fini = cv::Point(selected_lines[j][2], selected_lines[j][3]);

        // Condition to classify line as left side or right side
        if (slopes[j] > 0 && fini.x > img_center && ini.x > img_center) {
            right_lines.push_back(selected_lines[j]);
            right_flag = true;
        } else if (slopes[j] < 0 && fini.x < img_center && ini.x < img_center) {
            left_lines.push_back(selected_lines[j]);
            left_flag = true;
        }
        j++;
    }

    output[0] = right_lines;
    output[1] = left_lines;

    return output;
}   // Separate detected lines by their slope into right and left lines


std::vector<cv::Point> LaneDetector::regression(std::vector<std::vector<cv::Vec4i>> left_right_lines) {
    std::vector<cv::Point> output(4);
    cv::Point ini;
    cv::Point fini;
    cv::Point ini2;
    cv::Point fini2;
    cv::Vec4d right_line;
    cv::Vec4d left_line;
    std::vector<cv::Point> right_pts;
    std::vector<cv::Point> left_pts;

    // If right lines are being detected, fit a line using all the init and final points of the lines
    if (right_flag == true) {
        for (int k = 0; k < left_right_lines[0].size(); k++) {
            cv::Vec4i i = left_right_lines[0][k];
            ini = cv::Point(i[0], i[1]);
            fini = cv::Point(i[2], i[3]);

            right_pts.push_back(ini);
            right_pts.push_back(fini);
        }
        if (right_pts.size() > 0) {
            // The right line is formed here
            cv::fitLine(right_pts, right_line, CV_DIST_L2, 0, 0.01, 0.01);
            right_m = right_line[1] / right_line[0];
            right_b = cv::Point(right_line[2], right_line[3]);
        }
    }

    // If left lines are being detected, fit a line using all the init and final points of the lines
    if (left_flag == true) {
        for (int k = 0; k < left_right_lines[1].size(); k++) {
            cv::Vec4i i = left_right_lines[1][k];
            ini2 = cv::Point(i[0], i[1]);
            fini2 = cv::Point(i[2], i[3]);

            left_pts.push_back(ini2);
            left_pts.push_back(fini2);
        }

        if (left_pts.size() > 0) {
            // The left line is formed here
            cv::fitLine(left_pts, left_line, CV_DIST_L2, 0, 0.01, 0.01);

            left_m = left_line[1] / left_line[0];
            left_b = cv::Point(left_line[2], left_line[3]);
        }
    }

    // One the slope and offset points have been obtained, apply the line equation to obtain the line points
    int ini_y = img_height;
    int fin_y = img_height;

    double right_ini_x;
    double right_fin_x;
    if (right_pts.size() < 1) {
        right_ini_x = img_width;
        right_fin_x = img_width;
        fin_y = img_height;
    }
    else {
        //      std::cout << "\nPOINTS: " << right_pts << std::endl;
        fin_y = 45;
        right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
        right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;
        output[0] = cv::Point(right_ini_x, ini_y);
        output[1] = cv::Point(right_fin_x, fin_y);
    }


    double left_ini_x;
    double left_fin_x;
    if (left_pts.size() < 1) {
        left_ini_x = img_width;
        left_fin_x = img_width;
        fin_y = img_height;
    }
    else {
        fin_y = 45;
        left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
        left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;
        output[2] = cv::Point(left_ini_x, ini_y);
        output[3] = cv::Point(left_fin_x, fin_y);
    }

    cv::Point p1_right = output[0]; // "start"
    cv::Point p2_right = output[1]; // "end"

    cv::Point p1_left = output[2]; // "start"
    cv::Point p2_left = output[3]; // "end"

    return output;
}  // Get only one line for each side of the lane

//
std::vector<double> LaneDetector::findDegree(std::vector<cv::Point> pointVector) {
    cv::Point p1_right = pointVector[0]; // "start"
    cv::Point p2_right = pointVector[1]; // "end"

    cv::Point p1_left = pointVector[2]; // "start"
    cv::Point p2_left = pointVector[3]; // "end"
    double slope_right = (p2_right.y - p1_right.y) / (double)(p2_right.x - p1_right.x);
    double slope_left = (p2_left.y - p1_left.y) / (double)(p2_left.x - p1_left.x);
//    std::cout << "slope right: " << slope_right << ", slope left: " << slope_left << std::endl;
    double degree_right = abs((double)(atan(slope_left) * 180 / CV_PI));
    double degree_left = abs((double)(atan(slope_right) * 180 / CV_PI));

    return  { degree_left, degree_right };
}


std::string LaneDetector::predictTurn() {
    std::string output;
    double vanish_x;
    double thr_vp = 30;

    // The vanishing point is the point where both lane boundary lines intersect
    vanish_x = static_cast<double>(((right_m*right_b.x) - (left_m*left_b.x) - right_b.y + left_b.y) / (right_m - left_m));

    // The vanishing points location determines where is the road turning
    if (vanish_x < (img_center - thr_vp))
        output = "Left Turn";
    else if (vanish_x >(img_center + thr_vp))
        output = "Right Turn";
    else if (vanish_x >= (img_center - thr_vp) && vanish_x <= (img_center + thr_vp))
        output = "Straight";

    return output;
}

int LaneDetector::plotLane(cv::Mat inputImage, std::vector<cv::Point> lane, std::string turn) {
    std::vector<cv::Point> poly_points;
    cv::Mat cropped_img = inputImage(cv::Rect(0, 150, 320, 90));

    cv::line(cropped_img, lane[0], lane[1], cv::Scalar(0, 255, 255), 5, CV_AA);
    cv::line(cropped_img, lane[2], lane[3], cv::Scalar(0, 255, 255), 5, CV_AA);

    // Plot the turn message
    cv::putText(cropped_img, turn, cv::Point(50, 90), cv::FONT_HERSHEY_PLAIN, 2, cvScalar(0, 255, 0), 1,
                CV_AA);

    // Show the final output image
    cv::namedWindow("Lane", CV_WINDOW_AUTOSIZE);
    cv::imshow("Lane", cropped_img);
    return 0;

}
