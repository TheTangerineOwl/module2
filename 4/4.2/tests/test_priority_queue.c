#include "../unity/unity.h"
#include "../src/priority_queue.h"

PrQueue_t queue;

static inline void assertPrQueueEmpty(const PrQueue_t* q)
{
    TEST_ASSERT_NULL(q->head);
    TEST_ASSERT_NULL(q->tail);
    TEST_ASSERT_EQUAL(0, q->length);
}

static inline void assertSameNodes(
    const Node_t* actNode,
    const Node_t* expNodePtr,
    const unsigned char expPriority,
    const int expValue,
    const Node_t* expNext
)
{
    TEST_ASSERT_NOT_NULL(actNode);
    if (expNodePtr)
        TEST_ASSERT_EQUAL_PTR(expNodePtr, actNode);
    TEST_ASSERT_EQUAL_CHAR(expPriority, actNode->priority);
    TEST_ASSERT_EQUAL(expValue, actNode->value);
    if (expNext)
        TEST_ASSERT_EQUAL_PTR(expNext, actNode->next);
    else
        TEST_ASSERT_NULL(actNode->next);
}

void setUp(void)
{
    prQueueInit(&queue);
}

void tearDown(void)
{
    prQueueClear(&queue);
}

void test_prQueueInit(void)
{
    PrQueue_t testQ;
    PrQueue_t *initRes = prQueueInit(&testQ);
    
    TEST_ASSERT_NOT_NULL(initRes);
    TEST_ASSERT_EQUAL_PTR(&testQ, initRes);
    assertPrQueueEmpty(initRes);
}

void test_prQueueEnqueue(void)
{
    Node_t* add1, *add2, *add3;
    TEST_ASSERT_NULL(prQueueEnqueue(NULL, 128, 100));
    add1 = prQueueEnqueue(&queue, 128, 100);
    assertSameNodes(add1, queue.head, 128, 100, NULL);
    TEST_ASSERT_EQUAL_PTR(queue.tail, add1);
    TEST_ASSERT_EQUAL(1, queue.length);

    add2 = prQueueEnqueue(&queue, 0, 200);
    assertSameNodes(add2, queue.head, 0, 200, add1);
    assertSameNodes(add2->next, add1, 128, 100, NULL);
    TEST_ASSERT_EQUAL(2, queue.length);

    add3 = prQueueEnqueue(&queue, 255, 300);
    assertSameNodes(add3, queue.tail, 255, 300, NULL);
    TEST_ASSERT_EQUAL_PTR(add3, add1->next);
    TEST_ASSERT_EQUAL(3, queue.length);
}

void test_prQueueDequeue(void)
{
    TEST_ASSERT_NULL(prQueueDequeue(NULL));

    TEST_ASSERT_NULL(prQueueDequeue(&queue));

    prQueueEnqueue(&queue, 128, 100);
    Node_t* pop = prQueueDequeue(&queue);
    assertPrQueueEmpty(&queue);
    assertSameNodes(pop, NULL, 128, 100, NULL);
    pop = NULL;

    Node_t* add1, *add2, *add3;
    add1 = prQueueEnqueue(&queue, 128, 100);
    add2 = prQueueEnqueue(&queue, 255, 200);
    add3 = prQueueEnqueue(&queue, 0, 300);

    pop = prQueueDequeue(&queue);

    assertSameNodes(queue.head, add1, 128, 100, add2);
    assertSameNodes(queue.head->next, queue.tail, 255, 200, NULL);
    assertSameNodes(pop, add3, 0, 300, NULL);

    TEST_ASSERT_NOT_NULL(queue.tail);
    TEST_ASSERT_EQUAL(2, queue.length);
    pop = NULL;
}

void test_prQueueClear(void)
{
    TEST_ASSERT_NULL(prQueueClear(NULL));
    PrQueue_t* testQ = prQueueClear(&queue);
    TEST_ASSERT_NOT_NULL(testQ);
    TEST_ASSERT_EQUAL_PTR(&queue, testQ);
    assertPrQueueEmpty(testQ);

    prQueueEnqueue(&queue, 128, 100);
    TEST_ASSERT_NOT_NULL(prQueueClear(&queue));
    assertPrQueueEmpty(&queue);
}

void test_prQueueDeqExactPriority(void)
{
    TEST_ASSERT_NULL(prQueueDeqExactPriority(NULL, 0));
    TEST_ASSERT_NULL(prQueueDeqExactPriority(&queue, 0));
    Node_t* pop, *add1, *add2, *add3;

    // Удаление единственного элемента;
    add1 = prQueueEnqueue(&queue, 0, 100);
    pop = prQueueDeqExactPriority(&queue, 0);
    assertSameNodes(pop, add1, 0, 100, NULL);
    assertPrQueueEmpty(&queue);

    // Удаление элемента посередине;
    add1 = prQueueEnqueue(&queue, 0, 100);
    add2 = prQueueEnqueue(&queue, 128, 200);
    add3 = prQueueEnqueue(&queue, 255, 300);
    pop = prQueueDeqExactPriority(&queue, 128);
    assertSameNodes(pop, add2, 128, 200, NULL);
    TEST_ASSERT_EQUAL_PTR(queue.head, add1);
    TEST_ASSERT_EQUAL_PTR(queue.tail, add3);
    TEST_ASSERT_EQUAL_PTR(add1->next, add3);

    // Удаление первого элемента + при существовании другого узла с таким же приоритетом.
    add2 = prQueueEnqueue(&queue, 0, 200);
    pop = prQueueDeqExactPriority(&queue, 0);
    assertSameNodes(pop, add1, 0, 100, NULL);
    TEST_ASSERT_EQUAL_PTR(queue.head, add2);
    TEST_ASSERT_EQUAL_PTR(queue.tail, add3);
    TEST_ASSERT_EQUAL_PTR(add2->next, add3);

    // Удаление последнего элемента
    add1 = prQueueEnqueue(&queue, 0, 100);
    pop = prQueueDeqExactPriority(&queue, 255);
    assertSameNodes(pop, add3, 255, 300, NULL);
    assertSameNodes(queue.head, NULL, 0, 200, queue.tail);
    assertSameNodes(queue.tail, NULL, 0, 100, NULL);

    // Удаление отсутствующего приоритета
    TEST_ASSERT_NULL(prQueueDeqExactPriority(&queue, 255));
}

void test_prQueueDeqHighPriority(void)
{
    TEST_ASSERT_NULL(prQueueDeqHighPriority(NULL, 0));
    TEST_ASSERT_NULL(prQueueDeqHighPriority(&queue, 0));
    Node_t* pop, *add1, *add2, *add3;

    // Удаление единственного элемента;
    add1 = prQueueEnqueue(&queue, 0, 100);
    pop = prQueueDeqHighPriority(&queue, 0);
    assertSameNodes(pop, add1, 0, 100, NULL);
    assertPrQueueEmpty(&queue);

    // Удаление элемента посередине;
    add1 = prQueueEnqueue(&queue, 0, 100);
    add2 = prQueueEnqueue(&queue, 128, 200);
    add3 = prQueueEnqueue(&queue, 255, 300);
    pop = prQueueDeqHighPriority(&queue, 100);
    assertSameNodes(pop, add2, 128, 200, NULL);
    TEST_ASSERT_EQUAL_PTR(queue.head, add1);
    TEST_ASSERT_EQUAL_PTR(queue.tail, add3);
    TEST_ASSERT_EQUAL_PTR(add1->next, add3);

    // Удаление первого элемента + при существовании другого узла с таким же приоритетом.
    add2 = prQueueEnqueue(&queue, 0, 200);
    pop = prQueueDeqHighPriority(&queue, 0);
    assertSameNodes(pop, add1, 0, 100, NULL);
    TEST_ASSERT_EQUAL_PTR(queue.head, add2);
    TEST_ASSERT_EQUAL_PTR(queue.tail, add3);
    TEST_ASSERT_EQUAL_PTR(add2->next, add3);

    // Удаление последнего элемента
    add1 = prQueueEnqueue(&queue, 0, 100);
    pop = prQueueDeqHighPriority(&queue, 200);
    assertSameNodes(pop, add3, 255, 300, NULL);
    assertSameNodes(queue.head, NULL, 0, 200, queue.tail);
    assertSameNodes(queue.tail, NULL, 0, 100, NULL);

    // Удаление отсутствующего приоритета
    TEST_ASSERT_NULL(prQueueDeqHighPriority(&queue, 255));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_prQueueInit);
    RUN_TEST(test_prQueueClear);
    RUN_TEST(test_prQueueEnqueue);
    RUN_TEST(test_prQueueDequeue);
    RUN_TEST(test_prQueueDeqExactPriority);
    RUN_TEST(test_prQueueDeqHighPriority);
    return UNITY_END();
}