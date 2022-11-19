#include "create_array.h"
#include <stdlib.h>
#include <stdio.h>
char **create_array_2d(int width, int height)
{
    if(width<1 || height<1) return NULL;
    char **array=(char **)malloc( height*sizeof(int *));
    if(array==NULL)
    {
        return NULL;
    }

    for(int i=0; i<height; i++)
    {
        *(array+i) = (char *)malloc(sizeof (int) * width);
        if(*(array+i)==NULL)
        {
            for(int j=0; j<i; j++)
            {
                free(*(array+j));
            }
            free(array);
            return NULL;
        }
    }
    return array;
}


int create_array_2d_2(char ***ptr,int width, int height)
{

    if(width<1 || height<1 || ptr==NULL ) return 1;
    *ptr = create_array_2d(width,height);
    if(*ptr==NULL) return 2;
    return 0;
}
void destroy_array_2d_1(char **ptr, int height)
{
    if(height<1 || ptr==NULL) return;
    for(int j=height-1; j>=0; j--)
    {
        if(*(ptr+j)!=NULL)
            free(*(ptr+j));

    }
    free(ptr);
}
void destroy_array_2d(char ***ptr, int height)
{
    if(height<1 || ptr==NULL) return;
    if(*ptr!=NULL)
    {
        destroy_array_2d_1(*ptr,height);
    }
    *ptr=NULL;
}


void display_array_2d(char **ptr, int width, int height)
{
    if(ptr==NULL) return;
    if(width<1 || height<1) return;
    for(int i=0; i<height; i++)
    {

        for(int j=0; j<width; j++)
        {
            printf("%c ", *(*(ptr+i)+j));
        }
        printf("\n");
    }
}