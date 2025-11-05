#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dlfcn.h>
#include <fstream>

cv::Mat decode_screencap(unsigned char* data, size_t size);

extern "C" void get_grid_origin(int* x, int* y) {

    void* object{};
    char* error{};
    void* handler{};

    // open library
    handler = dlopen("utils/waydroid/build/get_screencap.so", RTLD_LAZY);
    // check if shared library was opened
    if (!handler) {
        std::cerr << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }
    dlerror(); // clear the error

    typedef unsigned char* (*get_screencap_t)(size_t*);
    auto get_screencap = (get_screencap_t)dlsym(handler, "get_screencap");

    //locate function
    object = (long*)dlsym(handler, "get_screencap");

    if ((error=dlerror()) != nullptr && !object) {
        std::cerr << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }

    size_t size = 0;
    unsigned char* data = get_screencap(&size);

    //716, 206, 1163, 654

    int gx1 = 716;
    int gy1 = 206;
    int gx2 = 1163;
    int gy2 = 654;
    int r = 0;
    int c = 0;

    if (data) {
        std::cout << "Got screenshot of " << size << " bytes!\n";

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

        cv::Mat img = decode_screencap(data, size);

        if (!img.empty()) {
            cv::Point topLeft(gx1, gy1);
            cv::Point bottomRight(gx2, gy2);

            float width = bottomRight.x - topLeft.x;
            float height = bottomRight.y - topLeft.y;
            std::cout << "Width: " << width << " Height: " << height << std::endl;
            cv::Rect roi(topLeft.x, topLeft.y, width, height);

            cv::Mat region = img(roi);
            // cv::imwrite("region.png", region);
            // std::cout << "Writing to screenshot of " << size << " bytes!\n";

            int cx = gx1 + ((float)c + 0.5 * (width/8.0));
            int cy = gy1 + ((float)r + 0.5 * (height/8.0));
            // std::cout << "Centre: " << cx << " " << cy << std::endl;
            *x = cx;
            *y = cy;
        }else {
            std::cerr << "Failed to decode screenshot!" << std::endl;
        }
        free(data);

    } else {
        std::cerr << "function returned null\n";
    }
    //close so
    dlclose(handler);

}

cv::Mat decode_screencap(unsigned char* data, size_t size) {
    std::vector<unsigned char> buffer(data, data + size);
    cv::Mat img = cv::imdecode(buffer, cv::IMREAD_COLOR);

    // check if image is empty
    if (img.empty()) {
        std::cerr << "Failed to decode image\n";
    }

    return img;
}
