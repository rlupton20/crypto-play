/* test.c */
#include <stdio.h>

#include "test.h"

char run_suite(testsuite ts)
{
  int i;
  char res = 1;
  char testresult;
  
  printf("\n** %s **\n", ts.suitename);

  for(i = 0; i<ts.numtests; ++i) {
    testresult = ts.tests[i].test();
    res = res && testresult;
    printf("%s: %s\n", ts.tests[i].description,
	   testresult ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m");
  }

  return res;
}
