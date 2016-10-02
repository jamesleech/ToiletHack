#ifdef UNIT_TEST

#include <stdio.h>
#include <unity.h>
#include "status.h"

Status status(1422);

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
 // clean stuff up here
}

void test_constructor(void) {
    TEST_ASSERT_EQUAL(1422, status.getNode());
}

int main()
{
  UNITY_BEGIN();

  RUN_TEST(test_constructor);

  UNITY_END();

  return 0;
}

#endif
