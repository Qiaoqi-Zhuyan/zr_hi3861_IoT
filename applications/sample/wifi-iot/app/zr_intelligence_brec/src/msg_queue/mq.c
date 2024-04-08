#include "mq.h"

void init_queue(MQueue* queue){
    queue->front = 0;
    queue->rear = 0;    
}

int is_empty(MQueue* queue){

    return (queue->front == queue->rear);

}

int is_full(MQueue* queue){

    return ((queue->rear + 1)% QUEUE_SIZE == queue->front);

}

void push_back(MQueue* queue, void* val){
        if(!is_full(queue)){
            queue->data[queue->rear] = val;
            queue->rear = (queue->rear + 1) % QUEUE_SIZE;
        }
}


void* pop(MQueue* queue){

    void* val = (void *)0;

    if (!is_empty(queue)){
        val = (queue->data[queue->front]);
        queue->front = (queue->front + 1) % QUEUE_SIZE;
    }


    return val;
}

void* peek(MQueue* queue){
    void* val = (void *)0;

    if (!is_empty(queue)){
        val = queue->data[queue->front];
    }


    return val;
}




