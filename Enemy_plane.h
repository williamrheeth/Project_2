#ifndef ENEMY_PLANE_H
#define ENEMY_PLANE_H

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

class Enemy_plane{
    public:
        Enemy_plane(int y_value, int x_value, int frame_value, int score, int hp) \
        : y(y_value), x(x_value), create_frame_enemy_plane(frame_value), score(score), hp(hp) {};
        int y, x;
        int cell_speed_enemy_plane=0;
        int buff_speed_enemy_plane;
        int create_frame_enemy_plane; //initialize when it creates
        int check_frame_enemy_plane;

        char type;
        int score;
        int hp;
        bool buff;
        vector<Bullet> bullet;
};

class Enemy_plane_1n: public Enemy_plane{
    public:
        Enemy_plane_1n(int y_value, int x_value, int frame_value) \
        : Enemy_plane(y_value, x_value, frame_value, 1, 10) {};
        Enemy_plane_1n(int y_value, int x_value, int frame_value, int score, int hp) \
        : Enemy_plane(y_value, x_value, frame_value, score, hp) {};
        int y, x;
        int create_frame_enemy_plane; //initialize when it creates
        int check_frame_enemy_plane=0;

        char type='n';
        bool buff=false;
};

class Enemy_plane_2r: public Enemy_plane_1n{
    public:
        Enemy_plane_2r(int y_value, int x_value, int frame_value) \
        : Enemy_plane_1n(y_value, x_value, frame_value, 2, 5) {};
        int cell_speed_enemy_plane=3;
        int create_frame_enemy_plane; //initialize when it creates
        int check_frame_enemy_plane=0;

        char type='r';
        bool buff=false;
};

class Enemy_plane_3s: public Enemy_plane_1n{
    public:
        Enemy_plane_3s(int y_value, int x_value, int frame_value) \
        : Enemy_plane_1n(y_value, x_value, frame_value, 3, 4) {};
        Enemy_plane_3s(int y_value, int x_value, int frame_value, int score, int hp) \
        : Enemy_plane_1n(y_value, x_value, frame_value, score, hp) {};

        int cell_speed_enemy_plane=9;
        int create_frame_enemy_plane; //initialize when it creates
        int check_frame_enemy_plane=0;

        char type='s';
        bool buff=false;
};

class Enemy_plane_4d: public Enemy_plane_3s{
    public:
        Enemy_plane_4d(int y_value, int x_value, int frame_value) \
        : Enemy_plane_3s(y_value, x_value, frame_value, 4, 5) {};
        int cell_speed_enemy_plane=3;
        int create_frame_enemy_plane; //initialize when it creates
        int check_frame_enemy_plane=0;

        char type='d';
        bool buff=false;
};

class Enemy_plane_5a: public Enemy_plane_3s{
    public:
        Enemy_plane_5a(int y_value, int x_value, int frame_value) \
        : Enemy_plane_3s(y_value, x_value, frame_value, 5, 8) {};
        int buff_speed_enemy_plane=6;
        int create_frame_enemy_plane; //initialize when it creates
        int check_frame_enemy_plane=0;

        char type='a';
        bool buff=false;
};

#endif