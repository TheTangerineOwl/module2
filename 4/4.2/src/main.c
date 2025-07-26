#include "priority_queue.h"

#include <stdio.h>

PrQueue_t queue;

void prQueuePrintNode(const Node_t* node)
{
    printf("[%u, %d]", node->priority, node->value);
}

void prQueuePrint(const PrQueue_t queue)
{
    Node_t* item = queue.head;
    while (item)
    {
        prQueuePrintNode(item);
        printf(", ");
        item = item->next;
    }
}

#define PRHT(q) \
        printf("\nHead is ");\
        prQueuePrintNode(q.head);\
        printf(" and tail is ");\
        prQueuePrintNode(q.tail);\
        puts("")

int main(void)
{
    if (!prQueueInit(&queue))
        return 1;
    for (int i = 0; i < 15; i++)
    {
        printf("\nAdding  [%d, %d], q's len: %ld\n", i % 10, i + 1, queue.length);
        prQueueEnqueue(&queue, i % 10, i + 1);
        prQueuePrint(queue);
    }
    prQueueDequeue(&queue);
    PRHT(queue);
    printf("\nQueue len: %ld, deq head\n", queue.length);
    prQueuePrint(queue);

    prQueueDeqExactPriority(&queue, 9);
    printf("\nQueue len: %ld, deq 9\n", queue.length);
    prQueuePrint(queue);
    PRHT(queue);

    prQueueDeqHighPriority(&queue, 3);
    printf("\nQueue len: %ld, ded >= 3\n", queue.length);
    prQueuePrint(queue);
    PRHT(queue);

    prQueueClear(&queue);
    printf("\nQueue len: %ld\n", queue.length);
    return 0;
}