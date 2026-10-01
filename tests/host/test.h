#ifndef TEST_H
#define TEST_H

#include <stdio.h>

static int test_failures = 0;
static int test_total = 0;

#define CHECK(cond) \
  do { \
    test_total++; \
    if (!(cond)) { \
      fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      test_failures++; \
    } \
  } while (0)

#define CHECK_EQ_INT(a, b) \
  do { \
    test_total++; \
    if ((a) != (b)) { \
      fprintf(stderr, "FAIL %s:%d: %d != %d\n", __FILE__, __LINE__, (a), (b)); \
      test_failures++; \
    } \
  } while (0)

#define TEST_MAIN_END() \
  do { \
    if (test_failures) { \
      fprintf(stderr, "FAILED: %d of %d checks\n", test_failures, test_total); \
    } else { \
      printf("ok: %d checks\n", test_total); \
    } \
    return test_failures ? 1 : 0; \
  } while (0)

#endif
