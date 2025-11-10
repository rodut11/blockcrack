#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "internal_vision.hpp"

extern "C" void get_block() {

    // 71, 206, 1163, 654
    // coordinates
    int gx1 = 715,
        gy1 = 206,
        gx2 = 1163,
        gy2 = 654;

    auto buffer = grab_screencap();
    cv::Mat img = decode_screencap(buffer);

    if (!img.empty()) {
        cv::Point topLeft(gx1, gy1);
        cv::Point bottomRight(gx2, gy2);

        int gwidth = bottomRight.x - topLeft.x;
        int gheight = bottomRight.y - topLeft.y;

        cv::Rect roi(topLeft.x, topLeft.y, gwidth, gheight);
        cv::Mat region = img(roi);

    }

}
