#ifdef UNIT_TEST

#include <stdio.h>
#include <unity.h>
#include <string.h>
#include "status.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
 // clean stuff up here
}

void test_constructor(void) {
  Status status(1422);
  TEST_ASSERT_EQUAL(1422, status.getNode());
}

void test_toJson(void) {
  Status status(1422);
  status.State = true;

  char* json;
  json = status.toJson();
  //TODO: figure out 'string' stuff...
  //test if the node 1422 value is in the string
  //test if the counter value is 1 in the string
  //test if the state is true in the string
  TEST_ASSERT_MESSAGE(1==1, json);
}

void test_fromJson(char *json) {

}

int main()
{
  UNITY_BEGIN();

  RUN_TEST(test_constructor);
  RUN_TEST(test_toJson);

  UNITY_END();

  return 0;
}

#endif
