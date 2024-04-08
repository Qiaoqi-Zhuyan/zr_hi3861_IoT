#ifndef ____MQ_H___
#define ____MQ_H___

#include "cmsis_os2.h"


#define QUEUE_SIZE 1024
typedef struct m_queue{
    void* data[QUEUE_SIZE];
    int front;
    int rear;
    osMutexId_t mutex;
}MQueue;

void init_queue(MQueue* queue);

int is_empty(MQueue* queue);

int is_full(MQueue* queue);

void push_back(MQueue* queue, void* val);

void* pop(MQueue* queue);

void* peek(MQueue* queue);





#endif /* ____MQ_H____ */
