/* shift_register.c */
/* Defines functions for shift_registers */

#include <stdlib.h>
#include <assert.h>
#include "shift_register.h"

#define BLOCKSIZE (8*sizeof(uint8_t)) /* Size of a byte in bits */

/* new_register(n) creates a new shift register
   containing n bits. n should be a multiple of 8. */
shift_register_t *new_register(int nbits)
{
  assert( nbits % BLOCKSIZE == 0 );

  /* Allocate a new shift_register, and set the number of bits. */
  shift_register_t *newregister;
  newregister = (shift_register_t *) malloc(sizeof(shift_register_t));
  /* INSERT SOME ERROR HANDLING HERE */

  newregister->nbits = nbits;

  /* Allocate the register bit blocks */
  int blocks = nbits / BLOCKSIZE;
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


uint8_t block_shift(shift_register_t *sr)
{
  assert( sr != NULL );
  uint8_t first = sr->reg[0];
  int blocks = (sr->nbits / BLOCKSIZE);
  int i;

  for(i = 0; i < blocks - 1; ++i)
    sr->reg[i] = sr->reg[i+1];

  /* zero the last block */
  sr->reg[i] = 0;
  
  return first;
}
