#include "priority_queue.h"
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

PrQueue_t queue;

void prQueuePrintNode(const Node_t* node)
{
    if (node)
        printf("[%u, %d]", node->priority, node->value);
    else
        printf("[-, -]");
}

void prQueuePrint(const PrQueue_t queue)
{
    Node_t* item = queue.head;
    while (item)
    {
        prQueuePrintNode(item);
        if (item->next)
            printf(", ");
        item = item->next;
    }
}

int main(void)
{
    if (!prQueueInit(&queue))
        return 1;
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        int num = rand() % 10;
        if (!prQueueEnqueue(&queue, num, i + 1))
            printf("Ничего не было добавлено! Длина очереди: %ld\n", queue.length);
        else
            printf("Добавлен  [%d, %d], длина очереди: %ld\n", num, i + 1, queue.length);
    }
    printf("Длина очереди: %ld, очередь: ", queue.length);
    prQueuePrint(queue);

    Node_t* pop;

    pop = prQueueDequeue(&queue);
    if (!pop)
        printf("\nНе удалось удалить элемент из головы!");
    else
    {
        printf("\nУдаленный элемент (голова): ");
        prQueuePrintNode(pop);
    }
    free(pop);
    printf("\nДлина очереди: %ld, очередь: ", queue.length);
    prQueuePrint(queue);

    pop = prQueueDeqExactPriority(&queue, 3);
    if (!pop)
        printf("\nНе удалось удалить элемент с приоритетом 3!");
    else
    {
        printf("\nУдаленный элемент (с приоритетом 3): ");
        prQueuePrintNode(pop);
    }
    free(pop);
    printf("\nДлина очереди: %ld, очередь: ", queue.length);
    prQueuePrint(queue);

    pop = prQueueDeqHighPriority(&queue, 5);
    if (!pop)
        printf("\nНе удалось удалить элемент с приоритетом больше 5!");
    else
    {
        printf("\nУдаленный элемент (приоритет >= 5): ");
        prQueuePrintNode(pop);
    }
    free(pop);
    printf("\nДлина очереди: %ld, очередь: ", queue.length);
    prQueuePrint(queue);

    pop = prQueueEnqueue(&queue, 0, 11);
    if (!pop)
        printf("\nНе удалось добавить элемент [0, 11]!");
    else
    {
        printf("\nДобавленный элемент: ");
        prQueuePrintNode(pop);
    }
    printf("\nДлина очереди: %ld, очередь: ", queue.length);
    prQueuePrint(queue);

    pop = prQueueEnqueue(&queue, 255, 12);
    if (!pop)
        printf("\nНе удалось добавить элемент [255, 12]!");
    else
    {
        printf("\nДобавленный элемент: ");
        prQueuePrintNode(pop);
    }
    printf("\nДлина очереди: %ld, очередь: ", queue.length);
    prQueuePrint(queue);

    prQueueClear(&queue);
    printf("\nДлина очереди после очистки: %ld\n", queue.length);
    return 0;
}