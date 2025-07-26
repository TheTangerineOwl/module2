#define UNITY_INCLUDE_DOUBLE
#define UNITY_SUPPORT_DOUBLE
#include "../unity/unity.h"
#include "../src/calc.h"
#include <math.h>

void setUp(void) { }

void tearDown(void) { }

void test_sum(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 4.0, *sum(2, 2.0, 2.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *sum(2, -1.0, 1.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -1.0, *sum(2, -1.0, 0.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -1.0, *sum(3, -2.0, 0.7, 0.3));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 2.0, *sum(4, -1.0, 2.5, 0.3, 0.2));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -3.0, *sum(5, -4.0, 0.3, 0.3, 0.3, 0.1));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *sum(2, -0.000001, 0.000001));
}

void test_sub(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 4.0, *sub(2, 2.0, -2.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *sub(2, -1.0, -1.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -1.0, *sub(2, -1.0, 0.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 4.0, *sub(2, 4.0, 0.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -0.4, *sub(3, 0.0, 0.7, -0.3));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -4.0, *sub(4, -1.0, 2.5, 0.3, 0.2));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 3.0, *sub(5, 4.0, 0.3, 0.3, 0.3, 0.1));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *sub(2, -0.000001, -0.000001));
}

void test_mult(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 4.0, *mult(2, 2.0, 2.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *mult(2, -1.0, 0.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -1.0, *mult(2, -1.0, 1.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 2.0, *mult(3, -2.0, 2.0, -0.5));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -15.0, *mult(4, -1.0, 2.5, 2.0, 3.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *mult(5, 4.0, 0.0, 0.3, 0.3, 0.1));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -0.00000000001, *mult(2, -0.00000000001, 1.0));
}

void test_divide(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 1.0, *divide(2, 2.0, 2.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 0.0, *divide(2, 0.0, 1.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -1.0, *divide(2, -1.0, 1.0));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 2.0, *divide(3, -2.0, 2.0, -0.5));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 1.0, *divide(4, 2.5, 0.5, 2.0, 2.5));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, 20.0, *divide(5, 4.0, 1.0, 0.5, 4.0, 0.1));
    TEST_ASSERT_DOUBLE_WITHIN(EPSILON, -1e-10, *divide(2, -1.0, 1e+10));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sum);
    RUN_TEST(test_sub);
    RUN_TEST(test_mult);
    RUN_TEST(test_divide);
    return UNITY_END();
}