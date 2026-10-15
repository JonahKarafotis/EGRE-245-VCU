#include <stdio.h>
#include <math.h>
#include "unity.h"
#include "mypow.h"
void setUp(void) {}
void tearDown(void) {}

// This Test Suite is for testing the mypow function with various inputs. The expected output is the result of x multiplied by itself y times.
void powtest1(void) {
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,8.0,mypow(2,3),"test 1-1");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,-8.0,mypow(-2,3),"test 1-2");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,234256.0,mypow(22,4),"test 1-3");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,4782969.0,mypow(3,14),"test 1-4");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1.0,mypow(1,1),"test 1-5");
    }

// This Test Suite is for testing the edge case of y being equal to 0. The expected output is 1 for all cases.
void powtest2(void) {
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(2,0),"test 2-1");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(6,0),"test 2-2");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(10,0),"test 2-3");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(100,0),"test 2-4");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(18,0),"test 2-5");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(0,0),"test 2-6");
    }

// This Test Suite is for testing the edge case of y being less than 0. The expected output is the result of 1 divided by x multiplied by itself y times.
void powtest3(void) {
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,1,mypow(1,-1),"test 3-1");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,0.25,mypow(2,-2),"test 3-2");
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01,0.03703703703,mypow(3,-3),"test 3-3");
    }

// This Test Suite is for testing the edge case's that are undefined.
// The Expected Result is that the function will Return NAN because we are raising 0 to a negative power which is undefined.
void powtest4(void) {
    TEST_ASSERT_FLOAT_IS_NAN(mypow(0, -3));
    TEST_ASSERT_FLOAT_IS_NAN(mypow(0, -6));
    TEST_ASSERT_FLOAT_IS_NAN(mypow(0, -3));
    TEST_ASSERT_FLOAT_IS_NAN(mypow(0, -4));
    }

int main ( void ){
    UNITY_BEGIN();

    // Run Our Test Suites
    RUN_TEST(powtest1);
    RUN_TEST(powtest2);
    RUN_TEST(powtest3);
    RUN_TEST(powtest4);

    return UNITY_END();
    }
