
#ifndef TEST_BUFFER_H
#define TEST_BUFFER_H
#include <stdio.h>
#include <stdlib.h>

struct death_point
{
    int x;
    int y;
    int stored_value;
};
struct buffer
{
    struct death_point **array;
    int size;
    int next_to_update;
};

struct buffer *create_buffer(int size);
void free_buffer(struct buffer **ptr);
void add_point(struct buffer *buffer, int x, int y, int amount);
void print_buffer(struct buffer *buffer);
void free_death_point(struct death_point ** point);
void remove_point(struct buffer *buffer, int x,int y);
void sort(struct buffer *buffer);
struct death_point *find_obj(struct buffer *buffer,int x, int y );
#endif //TEST_BUFFER_H
