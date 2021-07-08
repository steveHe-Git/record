/*
* Ｃ语言版的数组实现队列
*/
#ifndef QUEUE_ARRAY_H_
#define QUEUE_ARRAY_H_

#include <stdio.h>
#include <malloc.h>

typedef int ELEMTYPE;
static int* queue = NULL;
static int count = 0;

static void create_queue(int len) {
    queue =(int*)malloc(sizeof(ELEMTYPE) * len);
    count = 0;
}

static void destory_queue() {
    if (queue) {
        free(queue);
        queue = NULL;
    }
}

static void push(ELEMTYPE value) {
    queue[count] = value;
    count++;
}

static ELEMTYPE pop() {
    ELEMTYPE res;
    int i = 0;
    res = queue[0];
    count--; 
    while (i++ < count) {
      queue[i - 1] = queue[i]; 
    }
    return res;
}

static ELEMTYPE front() {
    return queue[0];
}

static int size() {
    return count;
}

static bool empty() {
    return (size() == 0);
}

#endif