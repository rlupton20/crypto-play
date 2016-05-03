/* This is a driver for the trivium cipher (to
   see how it works, etc. */

#include <stdio.h>
#include <stdlib.h>
#include "shift_register.h"
#include "trivium.h"


int main(int argc, char *argv[])
{
  int i;
  trivium_keystream *tks;

  
  printf("Trivium test driver.\n");

  run_suite(shift_register_tests);
  run_suite(trivium_testsuite);
  
  return 0;
}

