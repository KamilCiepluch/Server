//
// Created by root on 11/18/22.
//

#ifndef SERVER_2_BEAST_H
#define SERVER_2_BEAST_H
#include <stdio.h>
#include <stdlib.h>
#include "game_enums.h"
enum beast_move_options{RANDOM, CHASE_PLAYER};

struct beast
{
    int x;
    int y;
    enum beast_move_options moveOption;
    char map[5][5];
    enum effect effect;
    int beast_moved;
};
struct beast_buffer
{
    struct beast **array;
    int size;
    int current_size;
};
struct beast_buffer *create_beast_buffer(int size);
void free_beast_buffer(struct beast_buffer **ptr);
void print_beast_buffer(struct beast_buffer *buffer);
void add_beast(struct beast_buffer *buffer, int x, int y);
struct beast *find_beast(struct beast_buffer *buffer,int x, int y );
#endif //SERVER_2_BEAST_H
