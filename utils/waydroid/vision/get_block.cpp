#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "internal_vision.hpp"

struct Box {
    int x, y, w, h;
};

std::vector<Box> get_block_boxes(const cv::Mat& mask, int min_area = 64) {
    cv::Mat labels, stats, centroids;
    int num_labels = cv::connectedComponentsWithStats(mask, labels, stats, centroids, 8, CV_32S);

    std::vector<Box> boxes;
    for (int i = 1; i < num_labels; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area < min_area) continue;

        boxes.push_back({
            stats.at<int>(i, cv::CC_STAT_LEFT),
            stats.at<int>(i, cv::CC_STAT_TOP),
            stats.at<int>(i, cv::CC_STAT_WIDTH),
            stats.at<int>(i, cv::CC_STAT_HEIGHT)
        });
    }
    return boxes;
}

std::vector<std::vector<int>> detect_cells_grid(const cv::Mat& block_img, int n_rows = 8, int n_cols = 8, float fill_ratio_thresh = 0.55f) {
    // Make sure it's binary
    cv::Mat bin;
    if (block_img.channels() > 1)
        cv::cvtColor(block_img, bin, cv::COLOR_BGR2GRAY);
    else
        bin = block_img.clone();
    cv::threshold(bin, bin, 128, 255, cv::THRESH_BINARY);

    // Crop to non-zero area
    cv::Mat nonZeroCoords;
    cv::findNonZero(bin, nonZeroCoords);
    if (nonZeroCoords.empty())
        return std::vector<std::vector<int>>(n_rows, std::vector<int>(n_cols, 0));

    cv::Rect crop = cv::boundingRect(nonZeroCoords);
    cv::Mat cropped = bin(crop);

    int h = cropped.rows;
    int w = cropped.cols;
    int cell_h = h / n_rows;
    int cell_w = w / n_cols;

    std::vector<std::vector<int>> grid(n_rows, std::vector<int>(n_cols, 0));
    for (int r = 0; r < n_rows; ++r) {
        for (int c = 0; c < n_cols; ++c) {
            int y0 = r * cell_h;
            int x0 = c * cell_w;
            int h_eff = (r == n_rows - 1) ? h - y0 : cell_h;
            int w_eff = (c == n_cols - 1) ? w - x0 : cell_w;

            cv::Rect cell_rect(x0, y0, w_eff, h_eff);
            cv::Mat cell = cropped(cell_rect);

            double fill_ratio = (double)cv::countNonZero(cell) / (cell.rows * cell.cols + 1e-6);
            grid[r][c] = (fill_ratio > fill_ratio_thresh) ? 1 : 0;
        }
    }

    return grid;
}

extern "C" void get_block() {
    auto buffer = grab_screencap();
    cv::Mat img = decode_screencap(buffer);

    if (img.empty()) return;

    // Grid region
    int gx1 = 693, gy1 = 711, gx2 = 1183, gy2 = 855;
    cv::Rect roi(gx1, gy1, gx2 - gx1, gy2 - gy1);
    cv::Mat region = img(roi);

    // Threshold
    cv::Mat gray;
    cv::cvtColor(region, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, gray, 128, 255, cv::THRESH_BINARY_INV);

    std::vector<Box> merged_boxes = get_block_boxes(gray, 64);
    std::sort(merged_boxes.begin(), merged_boxes.end(), [](const Box& a, const Box& b) { return a.x < b.x; });
    if (merged_boxes.size() > 3) merged_boxes.resize(3);

    for (const Box& box : merged_boxes) {
        cv::Rect rect(box.x, box.y, box.w, box.h);
        cv::Mat block_img = region(rect);

        auto grid = detect_cells_grid(block_img, 8, 8, 0.55f);

        // Print bounding box
        std::cout << "Box: x=" << box.x << " y=" << box.y << " w=" << box.w << " h=" << box.h << "\n";

        // Print array
        for (const auto& row : grid) {
            for (int val : row)
                std::cout << val << ' ';
            std::cout << '\n';
        }
        std::cout << "----\n";
    }
}
