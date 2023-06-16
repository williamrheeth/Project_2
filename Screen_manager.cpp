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
        Bullet bullet = Bullet(this->my_plane.y-1+shot_frame, this->my_plane.x, check_frame, this->my_plane.level);
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
                
                if(iter->level==2){
                    iter->damage = 2;
                }

                if(iter->level>=3){
                    iter->damage = 3;
                }

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

        // enemy bullet
        for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); ) {
            for(auto bulletiter = (*iter)->enemy_bullet.begin(); bulletiter < (*iter)->enemy_bullet.end(); ) {
                if((*iter)->type == 's') {
                    if(bulletiter->y >= height-1) {
                        board[bulletiter->y][bulletiter->x]=' ';
                        (*iter)->enemy_bullet.erase(bulletiter);
                    } else {
                        if(bulletiter->y != 0 && curr_frame != 1) {
                            board[bulletiter->y][bulletiter->x]=' ';
                        }
                        bulletiter->y += 1;
                        board[bulletiter->y][bulletiter->x]='*';
                    
                        bulletiter++;
                    }
                }
                if((*iter)->type == 'd') {
                    if(bulletiter->y >= height-1) {
                        board[bulletiter->y][bulletiter->x]=' ';
                        (*iter)->enemy_bullet.erase(bulletiter);
                    } else {
                        if(bulletiter->y != 0 && curr_frame != 1) {
                            board[bulletiter->y][bulletiter->x]=' ';
                        }
                        bulletiter->y += 1;
                        if((*iter)->x <= 14) { //diagonally towards nearest wall
                            bulletiter->x -= 1;
                        } else if((*iter)->x >= 14) {
                            bulletiter->x += 1;
                        }

                        board[bulletiter->y][bulletiter->x]='*';
                    
                        bulletiter++;
                    }
                }
            }
            iter++;
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
                    Enemy_plane_1n* enemy = new Enemy_plane_1n(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(enemy);
                    this->frame_event[i] = 0;
                    break;
                case 'r':
                    Enemy_plane_2r* enemy = new Enemy_plane_2r(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(enemy);
                    this->frame_event[i] = 0;
                    break;
                case 's':
                    Enemy_plane_3s* enemy = new Enemy_plane_3s(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(enemy);
                    this->frame_event[i] = 0;
                    break;
                case 'd':
                    Enemy_plane_4d* enemy = new Enemy_plane_4d(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(enemy);
                    this->frame_event[i] = 0;
                    break;
                case 'a':
                    Enemy_plane_5a* enemy = new Enemy_plane_5a(y_event[i], x_event[i], curr_frame);
                    this->enemy_vector.push_back(enemy);
                    for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); iter++) {
                        if((*iter)->type == 'a') continue;
                        if((*iter)->y >= enemy->y-3 || (*iter)->y <= enemy->y+3) {
                            if((*iter)->x >= enemy->x-3 || (*iter)->x <= enemy->x+3)
                                (*iter)->buff = true;
                        }
                    }
                    this->frame_event[i] = 0;
                    break;
            }
        }
    }

    //Enemy part
    for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); ) {
        int cell_speed, buff_speed, create_frame, check_frame;
            cell_speed = (*iter)->cell_speed_enemy_plane;
            buff_speed = (*iter)->buff_speed_enemy_plane;
            create_frame = (*iter)->create_frame_enemy_plane;
            check_frame = (*iter)->check_frame_enemy_plane;
            
        if(cell_speed == 0) { // stationary enemy
            if((*iter)->y != 0 && curr_frame != 1) {
                board[(*iter)->y][(*iter)->x]=' ';
            }
            if((*iter)->buff == true) {
                board[(*iter)->y][(*iter)->x]=(char)(*iter)->type-32;
            } else {
                board[(*iter)->y][(*iter)->x]=(char)(*iter)->type;
            }

            if((*iter)->type == 'a') { // buffing enemy
                while((curr_frame-create_frame)/buff_speed - check_frame > 0) {
                    for(auto iter2 = this->enemy_vector.begin(); iter2 < this->enemy_vector.end(); iter2++) {
                        if((*iter2)->type == 'a') continue;
                        if((*iter2)->y >= (*iter)->y-3 || (*iter2)->y <= (*iter)->y+3) {
                            if((*iter2)->x >= (*iter)->x-3 || (*iter2)->x <= (*iter)->x+3)
                                (*iter2)->buff = true;
                        }
                    }
                }
            }
            
            iter++;
            (*iter)->check_frame_enemy_plane+=1;
            check_frame++;
            continue;
        }

        while ((curr_frame-create_frame)/cell_speed - check_frame > 0) { // moving enemy
            if((*iter)->y >= height-1) {
                board[(*iter)->y][(*iter)->x]=' ';
                delete *iter;
                this->enemy_vector.erase(iter);
            } else {
                if((*iter)->y != 0 && curr_frame != 1) {
                    board[(*iter)->y][(*iter)->x]=' ';
                }
                (*iter)->y += cell_speed;

                if((*iter)->buff == true) {
                    board[(*iter)->y][(*iter)->x]=(char)(*iter)->type-32;
                } else {
                    board[(*iter)->y][(*iter)->x]=(char)(*iter)->type;
                }

                int buff_val = 1;
                if((*iter)->buff == true) buff_val = 2;

                // enemy bullet part
                if((*iter)->type == 's') {
                    Enemy_Bullet bullet = Enemy_Bullet((*iter)->y+1, (*iter)->x, check_frame, buff_val);
                    (*iter)->enemy_bullet.push_back(bullet);
                }

                if((*iter)->type == 'd') {
                    Enemy_Bullet bullet = Enemy_Bullet((*iter)->y+1, (*iter)->x, check_frame, buff_val);
                    (*iter)->enemy_bullet.push_back(bullet);
                }

                iter++;
            }

            (*iter)->check_frame_enemy_plane += 1;
            check_frame++;
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