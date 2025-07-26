#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <malloc.h>

typedef struct Node_t Node_t;

struct Node_t
{
    unsigned char priority;
    int value;
    Node_t* next;
};

typedef struct
{
    Node_t* head;
    Node_t* tail;
    size_t length;
} PrQueue_t;

extern PrQueue_t queue;

PrQueue_t* prQueueInit(PrQueue_t* queue);
Node_t* prQueueEnqueue(PrQueue_t* queue, unsigned char priority, int value);
Node_t* prQueueDequeue(PrQueue_t* queue);
Node_t* prQueueDeqExactPriority(PrQueue_t* queue, int priority);
Node_t* prQueueDeqHighPriority(PrQueue_t* queue, int minPriority);
PrQueue_t* prQueueClear(PrQueue_t* queue);

void prQueuePrintNode(const Node_t* node);
void prQueuePrint(const PrQueue_t queue);

#endif