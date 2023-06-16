#ifndef BULLET_H
#define BULLET_H

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

class Bullet{
    public:
        Bullet(int y_value, int x_value, int frame_value, int level) \
        : y(y_value), x(x_value), create_frame_bullet(frame_value), level(level) {};
        int damage=1;
        int y, x;
        int create_frame_bullet; //initialize when it creates
        int check_frame_bullet;
        int level;
};

class Enemy_Bullet: public Bullet{
    public:
        Enemy_Bullet(int y_value, int x_value, int frame_value, int level) \
        : Bullet(y_value, x_value, frame_value, level) {};
        int damage=1;
        int y, x;
        int create_frame_bullet; //initialize when it creates
        int check_frame_bullet=0;
        int level=1;
};

#endif