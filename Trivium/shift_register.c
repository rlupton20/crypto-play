/* shift_register.c */
/* Defines functions for shift_registers */

#include <stdlib.h>
#include <assert.h>
#include "shift_register.h"
#include "test.h"

#include <stdio.h>

#define BLOCKSIZE (8*sizeof(uint8_t)) /* Size of a byte in bits */
#define numblocks(n) ((n) / BLOCKSIZE + ((n) % BLOCKSIZE ? 1 : 0))

/* new_register(n) creates a new shift register
   containing n bits. n should be a multiple of 8. */
shift_register_t *new_register(int nbits)
{
  assert( nbits > 8);

  /* Allocate a new shift_register, and set the number of bits. */
  shift_register_t *newregister;
  newregister = (shift_register_t *) malloc(sizeof(shift_register_t));
  /* INSERT SOME ERROR HANDLING HERE */

  newregister->nbits = nbits;

  /* Allocate the register bit blocks */
  int blocks = numblocks(nbits);
  newregister->reg = (uint8_t *) malloc(blocks);
  /* INSERT SOME ERROR HANDLING HERE */
}

void del_register(shift_register_t *sr)
{
  assert( sr != NULL );
  assert( sr->reg != NULL);

  free(sr->reg);
  free(sr);
}



/* get_bit(i) returns a non-zero value if bit i is
   set, and zero otherwise. */
char get_bit(int n, shift_register_t sr)
{
  assert(n < sr.nbits);

  int block = n / BLOCKSIZE;
  char bit = n % BLOCKSIZE;

  return (sr.reg[block] & (1 << bit));
}




void set_bit(int n, shift_register_t *sr)
{
  assert( sr != NULL );
  assert(n < sr->nbits);
  
  int block = n / BLOCKSIZE;
  char bit = n % BLOCKSIZE;  

  (sr->reg)[block] = (sr->reg)[block] | (1 << bit);
}



uint8_t get_block_at(int n, shift_register_t sr)
{
  assert( n + BLOCKSIZE <= sr.nbits );
  
  uint8_t higher = 0;
  uint8_t lower = 0;

  unsigned int block = n / BLOCKSIZE;
  uint8_t shift = n % BLOCKSIZE; 

  lower = (sr.reg[block] >> shift);
  /* If no bits are required from the block above, set higher
     to zero. Else get the correct bits. Note this also saves
     us reading outside the boundaries of the reg array if we 
     ask for the last block. */
  higher = shift ? (sr.reg[block+1] << (BLOCKSIZE - shift)) : 0;

  return (higher | lower);
}

void set_block_at(int n, uint8_t byte, shift_register_t *sr)
{
  assert( n + BLOCKSIZE <= sr->nbits);

  char shift = sr->nbits % BLOCKSIZE;
  int block = n / BLOCKSIZE;
  
  uint8_t maskl = 255 >> (BLOCKSIZE - shift);
  uint8_t maskh = 255 << shift;

  sr->reg[block] = (sr->reg[block] & maskl) | (byte << shift);
  if(shift)  /* i.e. we need to alter the byte above */
    sr->reg[block+1] = (sr->reg[block+1] & maskh) |
      (byte >> (BLOCKSIZE - shift));
  
}



uint8_t block_shift(shift_register_t *sr)
{
  assert( sr != NULL );
  uint8_t first = sr->reg[0];
  int blocks = numblocks(sr->nbits);
  int i;

  for(i = 0; i < blocks - 1; ++i)
    sr->reg[i] = sr->reg[i+1];

  /* zero the last block */
  sr->reg[i] = 0;
  
  return first;
}



/**           TESTS          **/


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

char set_block_at_test()
{
  shift_register_t *reg = new_register(13);
  uint8_t byte = 225;
  uint8_t res;

  set_block_at(5, byte, reg);
  res = get_block_at(5, *reg);

  del_register(reg);

  return (byte == res);
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

test tests[] = { {"get_bit test", get_bit_test },
		 {"get_block_at_test", get_block_at_test},
		 {"set_block_at test", set_block_at_test},
		 {"block_shift test", block_shift_test} };

testsuite shift_register_tests = { "shift_register.c tests",
				   tests,
				   sizeof(tests) / sizeof(test) };
