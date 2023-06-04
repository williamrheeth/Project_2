#ifndef MY_PLANE_H
#define MY_PLANE_H

#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <vector>
#include "Bullet.h"

using namespace std;

class My_plane{
    public:
        My_plane(int y_value, int x_value, int frame_value) \
        : y(y_value), x(x_value), create_frame_my_plane(frame_value) {};
        int y, x;
        int shot_frame_my_plane=1;
        int create_frame_my_plane; //initialize when it creates
        int check_frame_my_plane=0;
        vector<Bullet> bullet;
};
#endif