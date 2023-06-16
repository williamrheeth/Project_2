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
#include "Screen_manager.h"

using namespace std;

//move cursor
void cursorYX(int y, int x)
{
    COORD pos;
    pos.X = x;        
    pos.Y = y;            
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Screen_manager::render(){
    for(int j=0; j<height; j++){
        for(int i=0; i<width; i++){
            cursorYX(j, i);
            printf("%c", board[j][i]);
        }
    }
}

//print sharing things.
void Screen_manager::print_share(){
    //wall
    for(int i=0; i<width; i++){board[height-1][i]='w';}
    for(int j=0; j<height; j++){board[j][0]='w'; board[j][width-1]='w';}

    int shot_frame, create_frame, check_frame;

    //Bullet part
    shot_frame = this->my_plane.shot_frame_my_plane;
    create_frame = this->my_plane.create_frame_my_plane;
    check_frame = this->my_plane.check_frame_my_plane;
    while ((curr_frame-create_frame)/shot_frame - check_frame > 0){ //bullet create
        Bullet bullet = Bullet(this->my_plane.y-1+shot_frame, this->my_plane.x, check_frame);
        this->my_plane.bullet.push_back(bullet);

        for(auto iter=this->my_plane.bullet.begin(); iter<this->my_plane.bullet.end(); ){
            if(iter->y<=0){
                board[iter->y][iter->x]=' ';
                this->my_plane.bullet.erase(iter);
            }
            else{
                if(iter!=(this->my_plane.bullet.end()-1) && curr_frame!=1){
                    board[iter->y][iter->x]=' ';
                }
                iter->y -= shot_frame;
                
                if(this->my_plane.power_up == true) {
                    if(iter->level==1) {
                        board[iter->y][iter->x-1]='\'';
                        board[iter->y][iter->x]='\'';
                        board[iter->y][iter->x+1]='\'';
                    } else if(iter->level==2) {
                        board[iter->y][iter->x-1]='^';
                        board[iter->y][iter->x]='^';
                        board[iter->y][iter->x+1]='^';
                    } else if(iter->level>=3) {
                        board[iter->y][iter->x-1]='!';
                        board[iter->y][iter->x]='!';
                        board[iter->y][iter->x+1]='!';
                    }
                } else {
                    if(iter->level==1) {
                        board[iter->y][iter->x]='\'';
                    } else if(iter->level==2) {
                        board[iter->y][iter->x]='^';
                    } else if(iter->level>=3) {
                        board[iter->y][iter->x]='!';
                    }
                }
                iter++;
            }
        }
        this->my_plane.check_frame_my_plane+=1;
        check_frame++;
    }
    //Bullet part ends

    //Calling Event
    for(int i = 0; i < sizeof(this->frame_event); i++) {
        if(frame_event[i] == curr_frame) {
            switch (type_event[i]) {
                //Stores the address of enemy plane in enemy_vector
                case 'n':
                    Enemy_plane_1n enemy = Enemy_plane_1n(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(&enemy);
                    break;
                case 'r':
                    Enemy_plane_2r enemy = Enemy_plane_2r(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(&enemy);
                    break;
                case 's':
                    Enemy_plane_3s enemy = Enemy_plane_3s(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(&enemy);
                    break;
                case 'd':
                    Enemy_plane_4d enemy = Enemy_plane_4d(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(&enemy);
                    break;
                case 'a':
                    Enemy_plane_5a enemy = Enemy_plane_5a(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(&enemy);
                    for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); iter++) {
                        if((*iter)->type == 'a') continue;
                        if((*iter)->y >= enemy.y-3 || (*iter)->y <= enemy.y+3) {
                            if((*iter)->x >= enemy.x-3 || (*iter)->x <= enemy.x+3)
                                (*iter)->buff = true;
                        }
                    }
                    break;
            }
        }
    }

    //Enemy part
    for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); ) {
        if((*iter)->y >= height-1) {
            board[(*iter)->y][(*iter)->x]=' ';
            this->enemy_vector.erase(iter);
        } else {
            if((*iter)->y != 0 && curr_frame != 1) {
                board[(*iter)->y][(*iter)->x]=' ';
            }
            (*iter)->y += (*iter)->cell_speed_enemy_plane;

            if((*iter)->buff == true) {
                board[(*iter)->y][(*iter)->x]=(char)(*iter)->type-32;
            } else {
                board[(*iter)->y][(*iter)->x]=(char)(*iter)->type;
            }

            iter++;
        }
    }
}

//print when key didn't pressed
void Screen_manager::print(){
    board[this->my_plane.y][this->my_plane.x]='M';

    print_share();
}

//print when key pressed
void Screen_manager::print(int ch){ //ascii
    if(ch=='d'){ //right
        if(this->my_plane.y >=0 && this->my_plane.y <(height-1) && this->my_plane.x >0 && this->my_plane.x < (width-2)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.x +=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='a'){ //left
        if(this->my_plane.y >=0 && this->my_plane.y <(height-1) && this->my_plane.x >1 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.x -=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='w'){ //up
        if(this->my_plane.y >0 && this->my_plane.y <(height-1) && this->my_plane.x >0 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.y -=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }
    }
    else if(ch=='s'){ //down
        if(this->my_plane.y >=0 && this->my_plane.y <(height-2) && this->my_plane.x >0 && this->my_plane.x < (width-1)){
            board[this->my_plane.y][this->my_plane.x]=' ';
            this->my_plane.y +=1;
            board[this->my_plane.y][this->my_plane.x]='M';
        }    
    }

    print_share();
}