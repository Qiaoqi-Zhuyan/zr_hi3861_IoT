#include "mq.h"

void init_queue(MQueue* queue){
    queue->front = 0;
    queue->rear = 0;
    
}

int is_empty(MQueue* queue){
    return queue->front == queue->rear;
}

int is_full(MQueue* queue){
    return (queue->rear + 1) % QUEUE_SIZE == queue->front;
}

void push_back(MQueue* queue, void* val){

    if (is_full(queue)){
        return;
    }
    if(!is_full(queue)){
    queue->data[queue->rear] = val;
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    }
}


void* pop(MQueue* queue){
    // pthread_mutex_lock(&(queue->lock));

    // while (isEmpty(queue)) {
    //     pthread_cond_wait(&(queue->not_empty), &(queue->lock));
    // }
    void* val = (void *)0;
    if(!is_empty(queue)){
        val = queue->data[queue->front];
        queue->front = (queue->front + 1) % QUEUE_SIZE;
        return val;
    }
    else
        return val;
}

void* peek(MQueue* queue){

    // pthread_mutex_lock(&(queue->lock));

    // while (isEmpty(queue)) {
    //     pthread_cond_wait(&(queue->not_empty), &(queue->lock));
    // }
    void* val = (void *)0;
    if(!is_empty(queue)){
        val = queue->data[queue->front];
        return val;
    }
    else
        return val;
    // pthread_mutex_unlock(&(queue->lock));
}




