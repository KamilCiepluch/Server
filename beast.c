//
// Created by root on 11/18/22.
//

#include "beast.h"


struct beast** create_beasts_array(int size)
{
    //Tworzy tablice wsaznikow do strktury death_point
    if(size<1) return NULL;
    struct beast **array = calloc(size,sizeof (struct beast*));
    if(array==NULL) return NULL;
    for(int i=0; i<size; i++)
    {
        *(array+i) = NULL;
    }
    return array;
}
void free_beast(struct beast ** point)
{
    free(*point);
    *point= NULL;
}
void free_beasts_array(struct beast*** array,int size)
{
    if(array==NULL|| size<1) return ;
    if(*array==NULL) return ;
    for (int i = 0; i < size; ++i) {
        free_beast((*array)+i);
    }
    free(*array);
    *array=NULL;
}
struct beast* create_beast(int x, int y)
{
    if( x<0 || y<0) return NULL;
    struct beast *ptr = calloc(1, sizeof(struct beast));
    if(ptr==NULL) return NULL;
    ptr->x =x;
    ptr->y = y;
    ptr ->moveOption = RANDOM;
    return ptr;
}
struct beast_buffer *create_beast_buffer(int size)
{
    struct beast_buffer *ptr = calloc(1,sizeof (struct beast_buffer));
    if(ptr==NULL) return NULL;
    ptr->size =size;
    ptr->array = create_beasts_array(size);
    if(ptr->array==NULL)
    {
        free(ptr);
        return NULL;
    }
    return ptr;

}
void free_beast_buffer(struct beast_buffer **ptr)
{

    if(ptr==NULL) return;

    free_beasts_array( &(*ptr)->array,  (*ptr)->size);
    free(*ptr);
    *ptr=NULL;
}
void print_beast_buffer(struct beast_buffer *buffer)
{
    if(buffer==NULL) return;
    for(int i=0; i<buffer->size; i++)
    {
        if(buffer->array[i]!=NULL)
            printf("%d %d %d\n", buffer->array[i]->x, buffer->array[i]->y, buffer->array[i]->moveOption );
        else
            printf("NULL\n");
    }
}

struct beast *find_beast(struct beast_buffer *buffer,int x, int y )
{
    if(buffer==NULL) return NULL;
    for (int i = 0; i < buffer->size; ++i) {
        if(buffer->array[i]!=NULL)
        {
            if(buffer->array[i]->x == x && buffer->array[i]->y == y)
            {
                return buffer->array[i];
            }
        }
    }
    return NULL;
}
void add_beast(struct beast_buffer *buffer, int x, int y)
{
    if( buffer->next_to_update == buffer->size) return;
    int index = buffer->next_to_update;

    struct beast *ptr = find_beast(buffer,x,y);
    if(ptr!=NULL)
        return;

    buffer->array[index] = create_beast(x,y);
    buffer->next_to_update++;

}