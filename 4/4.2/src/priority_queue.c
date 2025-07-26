#include "priority_queue.h"

extern PrQueue_t queue;

PrQueue_t* prQueueInit(PrQueue_t* queue)
{
    queue = (PrQueue_t*)malloc(sizeof(PrQueue_t));
    queue->head = NULL;
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

Node_t* prQueueEnqueue(PrQueue_t* queue, unsigned char priority, int value)
{
    Node_t* node = (Node_t*)malloc(sizeof(Node_t));
    node->priority = priority;
    node->value = value;
    node->next = NULL;
    Node_t* current;
    if (!queue->head)
    {
        queue->head = node;
        queue->tail = node;
        queue->length++;
        return node;
    }
    else
        current = queue->head;
    while (current->next && current->next->priority <= priority)
        current = current->next;
    if (!current->next)
    {
        current->next = node;
        queue->tail->next = node;
        queue->tail = node;
    }
    else
    {
        Node_t* tailing = current->next;
        node->next = tailing;
        current->next = node;
    }
    queue->length++;
    return node;
}

Node_t* prQueueDequeue(PrQueue_t* queue)
{
    Node_t* head = queue->head;
    queue->head = head->next;
    if (!head->next)
        queue->tail = queue->head;
    queue->length--;
    return head;
}

Node_t* prQueueDeqExactPriority(PrQueue_t* queue, int priority)
{
    if (!queue->head)
        return queue->head;
    Node_t* item = queue->head, *buffer;
    if (item->priority == priority)
    {
        buffer = item;
        item = item->next;
        queue->head = item;
        if (!item->next)
            queue->tail = item;
        queue->length--;
        return buffer;
    }
    while (item->next && item->next->priority != priority)
        item = item->next;
    if (!item->next)
        return NULL;
    buffer = item->next;
    item->next = item->next->next;
    if (!item->next)
        queue->tail = item;
    queue->length--;
    return buffer;
}

Node_t* prQueueDeqHighPriority(PrQueue_t* queue, int minPriority)
{
    if (!queue->head)
        return queue->head;
    Node_t* item = queue->head, *buffer;
    if (item->priority >= minPriority)
    {
        buffer = item;
        item = item->next;
        queue->head = item;
        if (!item->next)
            queue->tail = item;
        queue->length--;
        return buffer;
    }
    while (item->next && item->next->priority < minPriority)
        item = item->next;
    if (!item->next)
        return NULL;
    buffer = item->next;
    item->next = item->next->next;
    if (!item->next)
        queue->tail = item;
    queue->length--;
    return buffer;
}

PrQueue_t* prQueueClear(PrQueue_t* queue)
{
    Node_t* item = queue->head;
    while (item->next)
        item = prQueueDequeue(queue);
    return queue;
}
