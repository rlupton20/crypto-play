/* shift_register.c */
/* Defines functions for shift_registers */

#include <stdlib.h>
#include <assert.h>
#include "shift_register.h"


/* new_register(n) creates a new shift register
   containing n bits. */
shift_register_t *new_register(int nbits)
{
  /* First take a measure of the number of bits in a block. */
  int bs = sizeof(block_t);

  /* Allocate a new shift_register, and set the number of bits. */
  shift_register_t *newregister;
  newregister = (shift_register_t *) malloc(sizeof(shift_register_t));
  /* INSERT SOME ERROR HANDLING HERE */

  newregister->nbits = nbits;

  /* Allocate the register bit blocks */
  int blocks = (nbits / bs) + (nbits % bs ? 1 : 0);
  newregister->reg = (block_t *) malloc(blocks * bs);
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

  int block = n / (8*sizeof(block_t));
  char bit = n % (8*sizeof(block_t));

  return (sr.reg[block] & (1 << bit));
}




void set_bit(int n, shift_register_t *sr)
{
  assert(n < sr->nbits);
  
  int block = n / (8*sizeof(block_t));
  char bit = n % (8*sizeof(block_t));

  (sr->reg)[block] = (sr->reg)[block] | (1 << bit);
}



block_t get_block_at(int n, shift_register_t sr)
{
  assert( n + 8*sizeof(block_t) <= sr.nbits );
  
  block_t higher = 0;
  block_t lower = 0;
  int block;
  char shift;

  block = n / (8*sizeof(block_t));
  shift = n % (8*sizeof(block_t));

  lower = lower | (sr.reg[block] >> shift);
  higher = higher | (sr.reg[block+1] << (8*sizeof(block_t) - shift));

  return (higher | lower);
}
