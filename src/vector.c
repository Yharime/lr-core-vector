/* vector.c —— 你要实现的地方 */

#include "vector.h"
#include <stdlib.h>
#include <string.h>

/*判断越界情况,容量为 0 要特别判断，检验申请是否成功*/
int vector_init(vector *v, size_t capacity) {
    if(capacity>=(SIZE_MAX/sizeof(int))){
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return -1;
    }//判断是否超出容量
    if(capacity==0){
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return 0;
    }//判断申请空间是否为零
    int *temp;
    temp = (int *)malloc(capacity*sizeof(int));
    if(temp == NULL){
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return -1;
    }//判断申请是否成功
    v->cap = temp + capacity;
    v->end = temp;
    v->data = temp;//处理数组的相关信息
    return 0;
}
/*检查数组是否为空*/
void vector_destroy(vector *v) {
    if(v->data == NULL)
    return;//为空则不用删除
    free(v->data);//释放空间
    v->cap = NULL;
    v->data = NULL;
    v->end = NULL;//置空指针
    return;
}
/*检查指针为空*/
size_t size(const vector *v) {
    if(v->data == NULL)
    return 0;
    else
    return (v->end - v->data);//求得数组的元素的数量
}
/*检查指针是否为空*/
size_t capacity(const vector *v) {
    if(v->cap == NULL)
    return 0;
    return (v->cap - v->data);//求数组的容量
}
/*也应该有检查指针是否为空*/
int empty(const vector *v) {
    if(v->end == v->data)
    return 1;//判断元素数是否与头指针相同
    else
    return 0;
}
/*越界与是否为空*/
int get(const vector *v, size_t index, int *out) {
    if(v->data == NULL)
    return -1;//判断数组是否为空
    if(index>=size(v))
        return -1;//判断越界
    *out = *(v->data + index);//根据索引得到数据
    return 0;
}
/*数组是否存在，索引越界*/
int set(vector *v, size_t index, int value) {
    if(v->cap == NULL)//判断数组是否存在
    return -1;
    if(index>=size(v))//判断越界
        return -1;
    *(v->data + index) = value;//对对应的数进行赋值
    return 0;
}
/*数组是否为空*/
int front(const vector *v, int *out) {
    if(empty(v))//检查数组是否为空
    return -1;
    *out = *v->data;//得到开头的数
    return 0;
}
/*数组是否为空*/
int back(const vector *v, int *out) {
    if(empty(v))//判断是否为空
    return -1;
    *out = *(v->end - 1);//得到尾部的数值
    return 0;
}
/*数组未申请判断，分别处理容量足够和容量不足的情况，不足时要新申请一倍空间，注意越界*/
int push_back(vector *v, int value) {
    if(v->cap == NULL){//当数组未申请的情况
        v->data = malloc(sizeof(int));
        if(v->data == NULL)
        return -1;//判断申请失败的情况
        *v->data = value; 
        v->end =v->data + 1;
        v->cap = v->end;//调整数组的相关信息
        return 0;
    }
    if((v->end+1)>v->cap){//判断是否超出容量
        if(size(v)*2<=(SIZE_MAX/sizeof(int))){//这里申请默认新申请一倍的空间
            int *temp;
            int n;
            n = size(v);//存储元素的个数，实际也就是容量，这里两者刚好相等
            temp = (int *)realloc(v->data,2*size(v)*sizeof(int));//在原有的空间上新申请空间，有可能会直接扩展或新申请后复制
            if(temp == NULL)
            return -1;//判断申请是否成功
            *(temp + n) = value;
            v->data = temp;
            v->end = temp + n + 1;
            v->cap = temp + 2*n;//调整数据
            return 0;
        }
        return -1;
    }
        else{
        *v->end = value;//容量足够的情况
        v->end++;
        return 0;
    }

}
/*判断数组是否为空*/
int pop_back(vector *v, int *out) {
     if(empty(v))
    return -1;
    *out = *(--v->end);//取出最后的元素，并删除，end本身指向下一个元素填充的位置
    return 0;
}
/*判断预留空间是否为0，判断是否需要预留空间*/
int reserve(vector *v, size_t capacity) {
    if(capacity == 0){//预留空间为零
        free(v->data);
        v->data = NULL;
        v->end = NULL;
        v->cap = NULL;
        return 0;
    }
    if(capacity<=(v->cap - v->data))
    return 0; //判断预留空间与现有空间的大小
    if(capacity>SIZE_MAX/sizeof(int))//判断是否超出极限
    return -1;
    int *temp;
    temp = (int *)realloc(v->data,sizeof(int)*capacity);//申请空间
    if(temp == NULL) 
    return -1;
    v->end = v->end - v->data + temp;//调整数组信息
    v->cap = capacity + temp;
    v->data = temp;
    return 0;
}
/*用的malloc，判断空间是否足够，复制原数据，并释放原内存*/
int shrink_to_fit(vector *v) {
     if(size(v) == 0){//判断没数据的情况
        free(v->data);
        v->cap = NULL;
        v->data = NULL;
        v->end = NULL;
        return 0;
    }
    if(SIZE_MAX/sizeof(int)<(v->end - v->data)*2)
        return -1;//判断是否越界，无法重新申请复制到的空间
    int *temp;
    temp = (int *)malloc((v->end - v->data)*sizeof(int));
    if(temp == NULL)
    return -1;//判断申请结果
    for(int i=0;i<(v->end - v->data);i++)
    *(temp + i) = *(v->data + i);//复制数据
    v->end = temp + (v->end - v->data);
    v->cap = v->end;
    free(v->data);//释放原空间
    v->data = temp;//调整数组信息
    return 0;

}
/*实际上只需调整尾部即可，不用重新设置*/
void clear(vector *v) {
    if(v->cap == NULL)
    return;
   memset(v->data,0,(v->end - v->data)*sizeof(int));//注意这个是按字节填充的，取中间的参数的低八位，赋给目标的每个字节，一半仅用于 0 和 -1 的赋值
    /*
    for(int i=0;i + v->data <v->end;i++){
        *(v->data + i) = 0;
    }
    */
   v->end = v->data;
    return;
}
