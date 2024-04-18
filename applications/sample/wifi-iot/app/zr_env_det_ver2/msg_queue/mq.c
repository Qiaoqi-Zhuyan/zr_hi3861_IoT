#include "mq.h"

void init_queue(MQueue* queue){
    queue->front = 0;
    queue->rear = 0;
    queue->mutex = osMutexNew(NULL);
    
}

int is_empty(MQueue* queue){
    int isEmpty = 0;
    if (osMutexAcquire(queue->mutex, osWaitForever) == osOK){
        isEmpty = (queue->front == queue->rear);
        osMutexRelease(queue->mutex);
    }
    return isEmpty;
}

int is_full(MQueue* queue){
    int isFull = 0;
    if(osMutexAcquire(queue->mutex, osWaitForever)== osOK){
        isFull = ((queue->rear + 1)% QUEUE_SIZE == queue->front);
        osMutexRelease(queue->mutex);
    }

    return isFull;
}

void push_back(MQueue* queue, void* val){

    if(osMutexAcquire(queue->mutex, osWaitForever) == osOK){
        if(!is_full(queue)){
            queue->data[queue->rear] = val;
            queue->rear = (queue->rear + 1) % QUEUE_SIZE;
        }
    }

    osMutexRelease(queue->mutex);

    // if (is_full(queue)){
    //     return;
    // }
    // if(!is_full(queue)){
    // queue->data[queue->rear] = val;
    // queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    // }
}


void* pop(MQueue* queue){

    void* val = (void *)0;
    if(osMutexAcquire(queue->mutex, osWaitForever)==osOK){
        if (queue->front != queue->rear){
            val = queue->data[queue->front];
            queue->front = (queue->front + 1) % QUEUE_SIZE;
        }

        osMutexRelease(queue->mutex);
    }

    return val;

    // void* val = (void *)0;
    // if(!is_empty(queue)){
    //     val = queue->data[queue->front];
    //     queue->front = (queue->front + 1) % QUEUE_SIZE;
    //     return val;
    // }
    // else
    //     return val;
}

void* peek(MQueue* queue){
    void* val = (void *)0;

    if(osMutexAcquire(queue->mutex, osWaitForever) == osOK){
        if (queue->front != queue->rear){
            val = queue->data[queue->front];
        }

        osMutexRelease(queue->mutex);
    }

    return val;

    // void* val = (void *)0;
    // if(!is_empty(queue)){
    //     val = queue->data[queue->front];
    //     return val;
    // }
    // else
    //     return val;
}



