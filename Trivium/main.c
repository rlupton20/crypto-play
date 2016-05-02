/* This is a driver for the trivium cipher (to
   see how it works, etc. */

#include <stdio.h>
#include <stdlib.h>
#include "shift_register.h"

char get_bit_test();
char get_block_at_test();

typedef struct _test {
  char *description;
  char (*test)(void);
} test;

test tests[] = { {"get_bit test", get_bit_test },
		 {"get_block_at_test", get_block_at_test} };
int n_tests = sizeof(tests) / sizeof(test);

int main(int argc, char *argv[])
{
  int i;
  
  printf("Trivium test driver.\n");
  for(i = 0; i < n_tests; ++i)
    printf("%s: %s\n", tests[i].description,
	   (tests[i].test() ? "PASS" : "FAIL"));
  
  return 0;
}


char get_bit_test()
{
  shift_register_t *reg;
  block_t isSet, isNSet;
  
  reg = new_register(24);

  set_bit(10, reg);
  isSet = get_bit(10, *reg);
  isNSet = get_bit(9, *reg) || get_bit(11, *reg);

  del_register(reg);
  return (isSet && !isNSet);
}

char get_block_at_test()
{
  shift_register_t *reg;
  block_t block;
  
  reg = new_register(24);
  /* Set bits on both sides of the byte boundary */
  set_bit(13, reg);
  set_bit(17, reg);

  block = get_block_at(10, *reg);
  del_register(reg);
  return (136 == block);
}
