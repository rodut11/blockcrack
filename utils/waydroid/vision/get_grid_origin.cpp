#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include "internal_vision.hpp"

extern "C" void get_grid_origin(int* x, int* y) {
    //716, 206, 1163, 654

    int gx1 = 716;
    int gy1 = 206;
    int gx2 = 1163;
    int gy2 = 654;
    int r = 0;
    int c = 0;

    // // test if image was succesfully read
    // std::ofstream output("out.png", std::ios::binary);
    // if (!output) {
    //     std::cerr << "failed to open file\n";
    //     free(data);
    //     dlclose(handler);
    //     return;
    // }
    //
    // output.write(reinterpret_cast<char*>(data), size);
    // output.close();
    //
    // std::cout << "Screenshot written to out.png\n";

    auto buffer = grab_screencap();

    cv::Mat img = decode_screencap(buffer);

    if (!img.empty()) {
        cv::Point topLeft(gx1, gy1);
        cv::Point bottomRight(gx2, gy2);

        int width = bottomRight.x - topLeft.x;
        int height = bottomRight.y - topLeft.y;
        // std::cout << "Width: " << width << " Height: " << height << std::endl;
        cv::Rect roi(topLeft.x, topLeft.y, width, height);

        cv::Mat region = img(roi);
        // cv::imwrite("region.png", region);
        // std::cout << "Writing to screenshot of " << size << " bytes!\n";


        // compute origin cell (0,0)
        *x = gx1 + ((float)c + 0.5 * (width/8.0));
        *y = gy1 + ((float)r + 0.5 * (height/8.0));
    }
}


