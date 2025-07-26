#include "priority_queue.h"

extern PrQueue_t queue;

PrQueue_t* prQueueInit(PrQueue_t* queue)
{
    if (!queue)
        queue = (PrQueue_t*)malloc(sizeof(PrQueue_t));
    if (!queue)
        return NULL;
    queue->head = queue->tail = NULL;
    queue->length = 0;
    return queue;
}

Node_t* prQueueEnqueue(PrQueue_t* queue, unsigned char priority, int value)
{
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    if (!node || !queue)
        return NULL;
    node->priority = priority;
    node->value = value;
    node->next = NULL;
    Node_t* current;
    if (!queue->head)
    {
        queue->head = queue->tail = node;
        queue->length = 1;
        return node;
    }
    
    if (priority < queue->head->priority) {
        node->next = queue->head;
        queue->head = node;
        queue->length++;
        return node;
    }

    current = queue->head;
    while (current->next && current->next->priority <= priority)
        current = current->next;

    node->next = current->next;
    current->next = node;
    if (!node->next)
        queue->tail = node;
    queue->length++;
    return node;
}

static Node_t* prQueueDequeueNode(PrQueue_t* queue, Node_t* prev, Node_t* current)
{
    if (!queue || !current)
        return NULL;
    if (prev)
        prev->next = current->next;
    else
        queue->head = current->next;
    if (!current->next)
        queue->tail = prev ? prev : queue->head;
    queue->length--;
    current->next = NULL;
    return current;
}

Node_t* prQueueDequeue(PrQueue_t* queue)
{
    if (!queue || !queue->head)
        return NULL;
    return prQueueDequeueNode(queue, NULL, queue->head);
}

Node_t* prQueueDeqExactPriority(PrQueue_t* queue, int priority)
{
    if (!queue || !queue->head)
        return NULL;
    Node_t* prev = NULL, *current = queue->head;

    while (current && current->priority != priority)
    {
        prev = current;
        current = current->next;
    }

    return current ? prQueueDequeueNode(queue, prev, current) : NULL;
}

Node_t* prQueueDeqHighPriority(PrQueue_t* queue, int minPriority)
{
    if (!queue || !queue->head)
        return NULL;

    Node_t* prev = NULL, *current = queue->head;

    while (current && current->priority < minPriority)
    {
        prev = current;
        current = current->next;
    }

    return current ? prQueueDequeueNode(queue, prev, current) : NULL;
}

PrQueue_t* prQueueClear(PrQueue_t* queue)
{
    if (!queue)
        return NULL;
    Node_t* item;
    while ((item = prQueueDequeue(queue)) != NULL)
        free(item);
    return queue;
}
