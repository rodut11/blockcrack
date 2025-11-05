//
// Created by rodut11 on 9/26/25.
//

#ifndef C_WAYDROID_COMM_H
#define C_WAYDROID_COMM_H

void waydroid_disconnect();
void waydroid_connect(const char *host);
void open_app(const char *input_app);
void get_grid(int grid[8][8]);
void get_block_coord();
void get_grid_pivot_point();

#endif //C_WAYDROID_COMM_H