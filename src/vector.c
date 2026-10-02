/* vector.c —— 你要实现的地方 */

#include "vector.h"
#include <stdlib.h>
#include <string.h>


int vector_init(vector *v, size_t capacity) {
    if(capacity>=(SIZE_MAX/sizeof(int))){
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return -1;
    }
    if(capacity==0){
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return 0;
    }
    int *temp;
    temp = (int *)malloc(capacity*sizeof(int));
    if(temp == NULL){
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return -1;
    }
    v->cap = temp + capacity;
    v->end = temp;
    v->data = temp;
    return 0;
}

void vector_destroy(vector *v) {
    if(v->data == NULL)
    return;
    free(v->data);
    v->cap = NULL;
    v->data = NULL;
    v->end = NULL;
    return;
}

size_t size(const vector *v) {
    if(v->data == NULL)
    return 0;
    else
    return (v->end - v->data);
}

size_t capacity(const vector *v) {
    if(v->cap == NULL)
    return 0;
    return (v->cap - v->data);
}

int empty(const vector *v) {
    if(v->end == v->data)
    return 1;
    else
    return 0;
}

int get(const vector *v, size_t index, int *out) {
    if(v->data == NULL)
    return -1;
    if(index>=size(v))
        return -1;
    *out = *(v->data + index);
    return 0;
}

int set(vector *v, size_t index, int value) {
    if(v->cap == NULL)
    return -1;
    if(index>=size(v))
        return -1;
    *(v->data + index) = value;
    return 0;
}

int front(const vector *v, int *out) {
    if(empty(v))
    return -1;
    *out = *v->data;
    return 0;
}

int back(const vector *v, int *out) {
    if(empty(v))
    return -1;
    *out = *(v->end - 1);
    return 0;
}

int push_back(vector *v, int value) {
    if(v->cap == NULL){
        v->data = malloc(sizeof(int));
        if(v->data == NULL)
        return -1;
        *v->data = value; 
        v->end =v->data + 1;
        v->cap = v->end;
        return 0;
    }
    if((v->end+1)>v->cap){
        if(size(v)*2<=(SIZE_MAX/sizeof(int))){
            int *temp;
            int n;
            n = size(v);
            temp = (int *)realloc(v->data,2*size(v)*sizeof(int));
            if(temp == NULL)
            return -1;
            *(temp + n) = value;
            v->data = temp;
            v->end = temp + n + 1;
            v->cap = temp + 2*n;
            return 0;
        }
        return -1;
    }
        else{
        *v->end = value;
        v->end++;
        return 0;
    }

}

int pop_back(vector *v, int *out) {
     if(empty(v))
    return -1;
    *out = *(--v->end);
    return 0;
}

int reserve(vector *v, size_t capacity) {
    if(capacity == 0){
        free(v->data);
        v->data = NULL;
        v->end = NULL;
        v->cap = NULL;
        return 0;
    }
    if(capacity<=(v->cap - v->data))
    return 0; 
    if(capacity>SIZE_MAX/sizeof(int))
    return -1;
    int *temp;
    temp = (int *)realloc(v->data,sizeof(int)*capacity);
    if(temp == NULL) 
    return -1;
    v->end = v->end - v->data + temp;
    v->cap = capacity + temp;
    v->data = temp;
    return 0;
}

int shrink_to_fit(vector *v) {
     if(size(v) == 0){
        free(v->data);
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return 0;
    }
    if(SIZE_MAX/sizeof(int)<(v->end - v->data)*2)
        return -1;
    int *temp;
    temp = (int *)malloc((v->end - v->data)*sizeof(int));
    if(temp == NULL)
    return -1;
    for(int i=0;i<(v->end - v->data);i++)
    *(temp + i) = *(v->data + i);
    v->end = temp + (v->end - v->data);
    v->cap = v->end;
    free(v->data);
    v->data = temp;
    return 0;

}

void clear(vector *v) {
    if(v->cap == NULL)
    return;
   memset(v->data,0,(v->end - v->data)*sizeof(int));
    /*
    for(int i=0;i + v->data <v->end;i++){
        *(v->data + i) = 0;
    }
    */
   v->end = v->data;
    return;
}
