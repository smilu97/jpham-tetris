#ifndef __WALLKICK_H__
#define __WALLKICK_H__

int wallkick_lookup_index[4][4] = {
    {0, 0, 0, 7},
    {1, 0, 2, 0},
    {0, 3, 0, 4},
    {6, 0, 5, 0},
};

// ref: https://tetris.wiki/Super_Rotation_System
int wallkick_table_jlstz[8][5][2] = {
    {{ 0,  0}, {-1,  0}, {-1,  1}, { 0, -2}, {-1, -2}},
    {{ 0,  0}, { 1,  0}, { 1, -1}, { 0,  2}, { 1,  2}},
    {{ 0,  0}, { 1,  0}, { 1, -1}, { 0,  2}, { 1,  2}},
    {{ 0,  0}, {-1,  0}, {-1,  1}, { 0, -2}, {-1, -2}},
    {{ 0,  0}, { 1,  0}, { 1,  1}, { 0, -2}, { 1, -2}},
    {{ 0,  0}, {-1,  0}, {-1, -1}, { 0,  2}, {-1,  2}},
    {{ 0,  0}, {-1,  0}, {-1, -1}, { 0,  2}, {-1,  2}},
    {{ 0,  0}, { 1,  0}, { 1,  1}, { 0, -2}, { 1, -2}},
};

int wallkick_table_i[8][5][2] = {
    {{ 0,  0}, {-2,  0}, { 1,  0}, {-2, -1}, { 1,  2}},
    {{ 0,  0}, { 2,  0}, {-1,  0}, { 2,  1}, {-1, -2}},
    {{ 0,  0}, {-1,  0}, { 2,  0}, {-1,  2}, { 2, -1}},
    {{ 0,  0}, { 1,  0}, {-2,  0}, { 1, -2}, {-2,  1}},
    {{ 0,  0}, { 2,  0}, {-1,  0}, { 2,  1}, {-1, -2}},
    {{ 0,  0}, {-2,  0}, { 1,  0}, {-2, -1}, { 1,  2}},
    {{ 0,  0}, { 1,  0}, {-2,  0}, { 1, -2}, {-2,  1}},
    {{ 0,  0}, {-1,  0}, { 2,  0}, {-1,  2}, { 2, -1}},
};

#endif