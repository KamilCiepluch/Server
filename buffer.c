//
// Created by root on 11/11/22.
//

#include "buffer.h"

struct death_point** create_death_point_array(int size)
{
    //Tworzy tablice wsaznikow do strktury death_point
    if(size<1) return NULL;
    struct death_point **array = calloc(size,sizeof (struct death_point*));
    if(array==NULL) return NULL;
    for(int i=0; i<size; i++)
    {
        *(array+i) = NULL;
    }
    return array;
}
void free_death_point(struct death_point ** point)
{
    free(*point);
    *point= NULL;
}
void free_death_point_array(struct death_point*** array,int size)
{
    if(array==NULL|| size<1) return ;
    if(*array==NULL) return ;
    for (int i = 0; i < size; ++i) {
        free_death_point((*array)+i);
    }
    free(*array);
    *array=NULL;
}
struct death_point* create_death_point(int x, int y, int ammount)
{
    if(ammount<1 || x<0 || y<0) return NULL;
    struct death_point *ptr = calloc(1, sizeof(struct death_point));
    if(ptr==NULL) return NULL;
    ptr->x =x;
    ptr->y = y;
    ptr->stored_value = ammount;
    return ptr;
}
struct buffer *create_buffer(int size)
{
    struct buffer *ptr = calloc(1,sizeof (struct buffer));
    if(ptr==NULL) return NULL;
    ptr->size =size;
    ptr->array = create_death_point_array(size);
    if(ptr->array==NULL)
    {
        free(ptr);
        return NULL;
    }
    return ptr;

}
void free_buffer(struct buffer **ptr)
{

    if(ptr==NULL) return;

    free_death_point_array( &(*ptr)->array,  (*ptr)->size);
    free(*ptr);
    *ptr=NULL;
}
void print_buffer(struct buffer *buffer)
{
    if(buffer==NULL) return;
    for(int i=0; i<buffer->size; i++)
    {
        if(buffer->array[i]!=NULL)
            printf("%d %d %d\n", buffer->array[i]->x, buffer->array[i]->y, buffer->array[i]->stored_value );
        else
            printf("NULL\n");
    }
}
int cmpfunc (const void * a, const void * b) {

    struct death_point *a1 = *(struct death_point**)a;
    struct death_point *a2 = *(struct death_point**)b;
    if(a1==NULL && a2!=NULL) return 1;
    if(a1!=NULL && a2==NULL) return -1;
    return 0;
}
struct death_point *find_obj(struct buffer *buffer,int x, int y )
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

int find_len(struct buffer *buffer)
{

    for (int i = 0; i < buffer->size; ++i) {
        if(buffer->array[i]==NULL) return i;
    }
    return -1;
}
void sort(struct buffer *buffer)
{
    qsort(buffer->array,buffer->size, sizeof(struct death_point *),cmpfunc );
    int i = find_len(buffer);
    if(i!=-1)
        buffer->next_to_update= i;
}
void remove_point(struct buffer *buffer, int x,int y)
{
    if(buffer==NULL) return;
    for (int i = 0; i <buffer->size ; ++i) {
        if(buffer->array[i]!=NULL)
        {
            if(buffer->array[i]->x ==x && buffer->array[i]->y==y)
            {
                free_death_point(&buffer->array[i]);
                buffer->array[i]=NULL;
                break;
            }
        }
    }
    sort(buffer);
}

void add_point(struct buffer *buffer, int x, int y, int amount)
{
    if(amount<1) return;
    int index = buffer->next_to_update;

    struct death_point *ptr = find_obj(buffer,x,y);
    if(ptr!=NULL)
    {
        ptr->stored_value+=amount;
        return;
    }

    if(buffer->array[index]!=NULL)
    {
        buffer->array[index]->x=x;
        buffer->array[index]->y=y;
        buffer->array[index]->stored_value=amount;
    }
    else
    {
        buffer->array[index] = create_death_point(x,y,amount);
    }
    buffer->next_to_update++;
    if(buffer->next_to_update==buffer->size)
    {
        buffer->next_to_update=0;
    }

}
