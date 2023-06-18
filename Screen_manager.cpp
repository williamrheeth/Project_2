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

    int shot_frame, create_frame, check_frame, cell_speed, buff_speed;

    //Bullet part
    shot_frame = this->my_plane.shot_frame_my_plane;
    create_frame = this->my_plane.create_frame_my_plane;
    check_frame = this->my_plane.check_frame_my_plane;
    while ((curr_frame-create_frame)/shot_frame - check_frame > 0){ //bullet create

        //Calling Event
        for(int i = 0; i < num_event; i++) {
            if(frame_event[i] == curr_frame) {
                char type = type_event[i];
                int y = y_event[i];
                int x = x_event[i];

                switch (type) {
                    case 'n': {
                        Enemy_plane_1n* enemy = new Enemy_plane_1n(y, x, curr_frame);
                        this->enemy_vector.push_back(enemy);
                        board[y][x] = 'n';
                        frame_event[i] = -1;
                        break;
                    }
                    case 'r': {
                        Enemy_plane_2r* enemy = new Enemy_plane_2r(y, x, curr_frame);
                        this->enemy_vector.push_back(enemy);
                        board[y][x] = 'r';
                        frame_event[i] = -1;
                        break;
                    }
                    case 's': {
                        Enemy_plane_3s* enemy = new Enemy_plane_3s(y, x, curr_frame);
                        this->enemy_vector.push_back(enemy);
                        board[y][x] = 's';
                        frame_event[i] = -1;
                        break;
                    }
                    case 'd': {
                        Enemy_plane_4d* enemy = new Enemy_plane_4d(y, x, curr_frame);
                        this->enemy_vector.push_back(enemy);
                        board[y][x] = 'd';
                        frame_event[i] = -1;
                        break;
                    }
                    case 'a': {
                        Enemy_plane_5a* enemy = new Enemy_plane_5a(y, x, curr_frame);
                        this->enemy_vector.push_back(enemy);
                        board[y][x] = 'a';
                        for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); iter++) {
                            if((*iter)->type == 'a') continue;
                            if((*iter)->x >= enemy->y-3 || (*iter)->y <= enemy->y+3) {
                                if((*iter)->x >= enemy->x-3 || (*iter)->x <= enemy->x+3)
                                    (*iter)->buff = true; // Buff enemies within the 7x7 unit zone
                            }
                        }
                        frame_event[i] = -1;
                        break;
                    }
                    case 'P': {
                        Power_up power_up = Power_up(y, x, curr_frame);
                        this->buff_vector.push_back(&power_up);
                        frame_event[i] = -1;
                        break;
                    }
                    case 'L': {
                        Level_up level_up = Level_up(y, x, curr_frame);
                        this->buff_vector.push_back(&level_up);
                        frame_event[i] = -1;
                        break;
                    }
                }
            }
        }


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
        if(!enemy_vector.empty()){
            
            for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); ) {
                if((*iter)->type == 's' || (*iter)->type == 'd') {
                    int level;
                    if((*iter)->buff) {
                        level = 2;
                    } else {
                        level = 1;
                    }

                    Enemy_Bullet enemy_bullet = Enemy_Bullet((*iter)->y+1, (*iter)->x, curr_frame, level);
                    (*iter)->enemy_bullet.push_back(enemy_bullet);

                    for(auto bulletiter = (*iter)->enemy_bullet.begin(); bulletiter != (*iter)->enemy_bullet.end(); ) {
                        if((*iter)->type == 's') {
                            if(bulletiter->y >= height-1) {
                                board[bulletiter->y][bulletiter->x]=' ';
                                bulletiter = (*iter)->enemy_bullet.erase(bulletiter);
                            } else {
                                if(bulletiter != (*iter)->enemy_bullet.end()-1 && curr_frame != 1) {
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
                                bulletiter = (*iter)->enemy_bullet.erase(bulletiter);
                            } else {
                                if(bulletiter != (*iter)->enemy_bullet.end()-1 && curr_frame != 1) {
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
                }       
                iter++;
            }
        }

        

        //-----------------------------------
        // Interaction between objects
        //-----------------------------------

        // Enemy plane vs My plane


        cout << "--- flag interaction" << endl;
    
        for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); iter++) {
            if((*iter)->y == this->my_plane.y && (*iter)->x == this->my_plane.x) {
                this->my_plane.hp -= 1;
            }
        }

        // Enemy bullet vs My plane
        for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); iter++) {
            for(auto iter2 = (*iter)->enemy_bullet.begin(); iter2 < (*iter)->enemy_bullet.end(); iter2++) {
                if((*iter2).y == this->my_plane.y && (*iter2).x == this->my_plane.x) {
                    this->my_plane.hp -= (*iter2).level;
                    (*iter)->enemy_bullet.erase(iter2);
                }
            }
        }

        // My bullet vs Enemy
        for(auto iter = this->my_plane.bullet.begin(); iter != this->my_plane.bullet.end(); iter++) {
            for(auto iter2 = this->enemy_vector.begin(); iter2 < this->enemy_vector.end(); iter2++) {
                if((*iter).y == (*iter2)->y && (*iter).x == (*iter2)->x) {
                    (*iter2)->hp -= (*iter).level;
                    this->my_plane.bullet.erase(iter);
                }
            }
        }

        // My plane getting buff
        for(auto iter = this->buff_vector.begin(); iter < this->buff_vector.end(); iter++) {
            if((*iter)->y == this->my_plane.y && (*iter)->x == this->my_plane.x) {
                switch ((*iter)->type) {
                    case 'P':
                        this->my_plane.power_up = true;
                        break;
                    case 'L':
                        this->my_plane.level += 1;
                        break;
                }
                this->buff_vector.erase(iter);
            }
        }
        
        this->my_plane.check_frame_my_plane+=1;
        check_frame++;
    }

    // enemy deploy/move

    if(!(this->enemy_vector.empty())) {
    for(auto iter = this->enemy_vector.begin(); iter < this->enemy_vector.end(); ) {
        switch ((*iter)->type) {
            case 'n':
                cell_speed = (*iter)->cell_speed_enemy_plane;
                create_frame = (*iter)->create_frame_enemy_plane;
                check_frame = (*iter)->check_frame_enemy_plane;
                if(check_frame == 0) {
                    board[(*iter)->y][(*iter)->x]='n'; // buff separately
                    (*iter)->check_frame_enemy_plane += 1;
                }
                break;
            case 'r':
                cell_speed = (*iter)->cell_speed_enemy_plane;
                create_frame = (*iter)->create_frame_enemy_plane;
                check_frame = (*iter)->check_frame_enemy_plane;
                if((curr_frame-create_frame) / cell_speed - check_frame > 0) {
                    board[(*iter)->y][(*iter)->x]=' ';
                    (*iter)->y += 1;
                    board[(*iter)->y][(*iter)->x]='r';
                    (*iter)->check_frame_enemy_plane += 1;
                }
                break;
            case 's':
                cell_speed = (*iter)->cell_speed_enemy_plane;
                create_frame = (*iter)->create_frame_enemy_plane;
                check_frame = (*iter)->check_frame_enemy_plane;
                if((curr_frame-create_frame) / cell_speed - check_frame > 0) {
                    board[(*iter)->y][(*iter)->x]=' ';
                    (*iter)->y += 1;
                    board[(*iter)->y][(*iter)->x]='s';
                    (*iter)->check_frame_enemy_plane += 1;
                }
                break;
            case 'd':
                cell_speed = (*iter)->cell_speed_enemy_plane;
                create_frame = (*iter)->create_frame_enemy_plane;
                check_frame = (*iter)->check_frame_enemy_plane;
                if((curr_frame-create_frame) / cell_speed - check_frame > 0) {
                    board[(*iter)->y][(*iter)->x]=' ';
                    (*iter)->y += 1;
                    board[(*iter)->y][(*iter)->x]='d';
                    (*iter)->check_frame_enemy_plane += 1;
                }
                break;
            case 'a':
                cell_speed = (*iter)->cell_speed_enemy_plane;
                create_frame = (*iter)->create_frame_enemy_plane;
                check_frame = (*iter)->check_frame_enemy_plane;
                if(check_frame == 0) {
                    board[(*iter)->y][(*iter)->x]='a';
                    (*iter)->check_frame_enemy_plane += 1;
                }
                break;
            }
        iter++;
        }
    }

    //Bullet part ends
    
}

//print when key didn't pressed
void Screen_manager::print(){
    board[this->my_plane.y][this->my_plane.x]='M';

    print_share();
}

//print when key pressed
void Screen_manager::print(int ch) { // ascii
    // Clear previous player position
    board[this->my_plane.y][this->my_plane.x] = ' ';

    // Update player position based on input
    if (ch == 'd') { // right
        if (this->my_plane.x < width - 2) {
            this->my_plane.x += 1;
        }
    } else if (ch == 'a') { // left
        if (this->my_plane.x > 1) {
            this->my_plane.x -= 1;
        }
    } else if (ch == 'w') { // up
        if (this->my_plane.y > 0) {
            this->my_plane.y -= 1;
        }
    } else if (ch == 's') { // down
        if (this->my_plane.y < height - 2) {
            this->my_plane.y += 1;
        }
    }

    // Print the updated game state
    print_share();
}
