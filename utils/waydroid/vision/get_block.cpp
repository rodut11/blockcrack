#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "internal_vision.hpp"
#include "../../../include/blocks.h"

struct Box {
    int x, y, w, h;
};

cv::Mat remove_small_components(const cv::Mat& mask, int min_area);
cv::Mat detect_cells_grid(const cv::Mat& block_img, int cell_w, int cell_h, float fill_ratio_thresh);
std::vector<Box> get_block_boxes(const cv::Mat& mask, int min_area);

extern "C" void get_block() {
    auto buffer = grab_screencap();
    cv::Mat img = decode_screencap(buffer);

    if (!img.empty()) {
        // coordinates (top-left and bottom-right)
        int gx1 = 693;
        int gy1 = 691;
        int gx2 = 1183;
        int gy2 = 855;

        // calculate region
        cv::Point topLeft(gx1, gy1);
        cv::Point bottomRight(gx2, gy2);

        int gwidth = bottomRight.x - topLeft.x;
        int gheight = bottomRight.y - topLeft.y;
        //std::cout << "Width: " << gwidth << " Height: " << gheight << std::endl;
        cv::Rect roi(topLeft.x, topLeft.y, gwidth, gheight);

        cv::Mat region = img(roi);

        cv::cvtColor(region, region, cv::COLOR_BGR2GRAY);
        cv::threshold(region, region, 127, 255, cv::THRESH_BINARY_INV);

        // morphological cleaning, whatever is that lol (translated from get_block.py)

        cv::Mat kernel_close = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::Mat cleaned;
        cv::morphologyEx(region, cleaned, cv::MORPH_CLOSE, kernel_close, cv::Point(-1,-1), 1);
        // cv::Mat_<uchar> creates a 3x3 matrix and << fills the matrix row by row
        cv::Mat dilate_kernel = (cv::Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
        cv::dilate(cleaned, cleaned, dilate_kernel, cv::Point(-1,-1), 2);

        cleaned = remove_small_components(cleaned, 40);

    }
}

cv::Mat remove_small_components(const cv::Mat& mask, int min_area) {

    cv::Mat labels, stats, centroids;
    int num_labels = cv::connectedComponentsWithStats(mask, labels, stats, centroids, 8);
    cv::Mat out = cv::Mat::zeros(mask.size(), mask.type());
    for (int i = 1; i < num_labels; i++) {
        if (stats.at<int>(i, 4) >= min_area) {
            out.setTo(255, labels == i);
        }
    }
    return out;
}

std::vector<Box> get_block_boxes(const cv::Mat& mask, int min_area) {
    cv::Mat labels, stats, centroids;
    int num_labels = cv::connectedComponentsWithStats(mask, labels, stats, centroids, 8);

    std::vector<Box> boxes;

    for (int i = 1; i < num_labels; i++) {
        int x    = stats.at<int>(i, 0);
        int y    = stats.at<int>(i, 1);
        int w    = stats.at<int>(i, 2);
        int h    = stats.at<int>(i, 3);
        int area = stats.at<int>(i, 4);

        if (area >= min_area) {
            boxes.push_back({x, y, w, h});
        }
    }
    return boxes;
}

cv::Mat detect_cells_grid(const cv::Mat& block_img, int cell_w, int cell_h, float fill_ratio_thresh = 0.30f) {
    std::vector<cv::Point> nonZeroPoints;
    cv::findNonZero(block_img, nonZeroPoints);

    if (nonZeroPoints.empty()) {
        return cv::Mat::zeros(1, 1, CV_8U);
    }

    int x_min = block_img.cols,
        x_max = 0,
        y_min = block_img.rows,
        y_max = 0;
    for (const auto& pt : nonZeroPoints) {
        x_min = std::min(x_min, pt.x);
        x_max = std::max(x_max, pt.x);
        y_min = std::min(y_min, pt.y);
        y_max = std::max(y_max, pt.y);
    }

    cv::Rect roi(x_min, y_min, x_max - x_min + 1, y_max - y_min + 1);
    cv::Mat cropped = block_img(roi);

    int h = cropped.rows;
    int w = cropped.cols;
    int n_rows = std::max(1, h / cell_h);
    int n_cols = std::max(1, w / cell_w);
    cropped = cropped(cv::Rect(0, 0, n_cols * cell_w, n_rows * cell_h));

    cv::Mat grid = cv::Mat::zeros(n_rows, n_cols, CV_8U);

    for (int r = 0; r < n_rows; ++r) {
        for (int c = 0; c < n_cols; ++c) {
            cv::Rect cell_rect(c * cell_w, r * cell_h, cell_w, cell_h);
            cv::Mat cell = cropped(cell_rect);

            int nonZeroCount = cv::countNonZero(cell);
            if (static_cast<float>(nonZeroCount) / (cell_w * cell_h) > fill_ratio_thresh) {
                grid.at<uchar>(r, c) = 1;
            }
        }
    }

    return grid; // clean, single return
}