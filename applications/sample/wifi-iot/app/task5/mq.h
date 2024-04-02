#ifndef ____MQ_H___
#define ____MQ_H___

#define QUEUE_SIZE 256
typedef struct m_queue{
    void* data[QUEUE_SIZE];
    int front;
    int rear;
    // pthread_mutex_t lock;
    // pthread_cond_t not_empty;
    // pthread_cond_t not_full;

}MQueue;

void init_queue(MQueue* queue);

int is_empty(MQueue* queue);

int is_full(MQueue* queue);

void push_back(MQueue* queue, void* val);

void* pop(MQueue* queue);

void* peek(MQueue* queue);

#endif /* ____MQ_H____ */
