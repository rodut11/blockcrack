#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlfcn.h>
#include <fstream>

cv::Mat decode_screencap(const std::vector<unsigned char>& buffer);
std::vector<unsigned char> grab_screencap();

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

        float width = bottomRight.x - topLeft.x;
        float height = bottomRight.y - topLeft.y;
        // std::cout << "Width: " << width << " Height: " << height << std::endl;
        cv::Rect roi(topLeft.x, topLeft.y, width, height);

        cv::Mat region = img(roi);
        // cv::imwrite("region.png", region);
        // std::cout << "Writing to screenshot of " << size << " bytes!\n";

        int cx = gx1 + ((float)c + 0.5 * (width/8.0));
        int cy = gy1 + ((float)r + 0.5 * (height/8.0));
            // std::cout << "Centre: " << cx << " " << cy << std::endl;
        *x = cx;
        *y = cy;
    }
}

cv::Mat decode_screencap(const std::vector<unsigned char>& buffer) {
    cv::Mat img = cv::imdecode(buffer, cv::IMREAD_COLOR);

    // check if image is empty
    if (img.empty()) {
        std::cerr << "Failed to decode image\n";
    }

    return img;
}

std::vector<unsigned char> grab_screencap() {
    FILE *fp = popen("adb exec-out screencap -p", "r");
    if (!fp) {
        std::cerr << "Failed to run popen\n";
        exit(EXIT_FAILURE);
    }

    std::vector<unsigned char> buffer;
    u_char tmp[4096];
    size_t n;

    while ((n = fread(tmp, 1, sizeof(tmp), fp)) > 0) {
        buffer.insert(buffer.end(), tmp, tmp+n);
    }
    pclose(fp);
    return buffer;
}
