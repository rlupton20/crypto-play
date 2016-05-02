/* test.h */
/* Define basic testsuite types and functions. */
#ifndef _TRIVIUM_TEST_H
#define _TRIVIUM_TEST_H

typedef struct _test {
  char *description;
  char (*test)(void);
} test;

typedef struct _test_suite {
  char *suitename;
  test *tests;
  int numtests;
} testsuite;

char run_suite(testsuite);

#endif
