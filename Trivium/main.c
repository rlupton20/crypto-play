/* This is a driver for the trivium cipher (to
   see how it works, etc. */

#include <stdio.h>
#include <stdlib.h>
#include "shift_register.h"
#include "trivium.h"

char get_bit_test();
char get_block_at_test();
char block_shift_test();

typedef struct _test {
  char *description;
  char (*test)(void);
} test;

test tests[] = { {"get_bit test", get_bit_test },
		 {"get_block_at_test", get_block_at_test},
		 {"block_shift test", block_shift_test} };
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
  uint8_t isSet, isNSet;
  
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
  uint8_t block;
  
  reg = new_register(24);
  /* Set bits on both sides of the byte boundary */
  set_bit(13, reg);
  set_bit(17, reg);

  block = get_block_at(10, *reg);
  del_register(reg);
  return (136 == block);
}

char block_shift_test()
{
  shift_register_t *reg = new_register(24);
  uint8_t first;
  uint8_t result;

  set_bit(3, reg);
  set_bit(11, reg);
  set_bit(18, reg);

  first = block_shift(reg);
  result = (first == 8 && get_block_at(0,*reg) == 8
	    && get_block_at(16,*reg) == 0);
  
  del_register(reg);

  return result;
}
