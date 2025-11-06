#ifndef CXX_VISION_H
#define CXX_VISION_H

#ifdef __cplusplus
extern "C" {
#endif

    // C-callable function
    void get_grid_origin(int* x, int* y);
    void get_grid(int grid[8][8]);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CXX_VISION_H
