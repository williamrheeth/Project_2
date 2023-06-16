#ifndef BUFF_H
#define BUFF_H

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
#include "My_plane.h"

class Buff{
    public:
        Buff(int y_value, int x_value, int frame_value, char type) \
        : y(y_value), x(x_value), create_frame_buff(frame_value), type(type) {};
        int y, x;
        int create_frame_buff; //initialize when it creates
        char type;
        virtual void doBuff(My_plane &my_plane)=0;
};

class Power_up: public Buff{
    public:
        Power_up(int y_value, int x_value, int frame_value) \
        : Buff(y_value, x_value, frame_value, 'P') {};

        virtual void doBuff(My_plane &my_plane) {
            my_plane.power_up=true;
        }
};

class Level_up: public Buff{
    public:
        Level_up(int y_value, int x_value, int frame_value) \
        : Buff(y_value, x_value, frame_value, 'L') {};

        virtual void doBuff(My_plane &my_plane) {
            my_plane.level++;
        }
};

#endif