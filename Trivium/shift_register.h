/* shift_register.h */
/* Contains basic functions for dealing with long blocks of bytes
   which are intended for use as a shift register */

#include <stdint.h>
#include "test.h"

#ifndef _SHIFTREG
#define _SHIFTREG

typedef struct s_shift_register {
  unsigned int nbits;
  uint8_t *reg;
} shift_register_t;

shift_register_t *new_register(int nbytes);
void del_register(shift_register_t *sr);

char get_bit(int n, shift_register_t sr);
void set_bit(int n, shift_register_t *sr);

uint8_t get_block_at(int n, shift_register_t sr);
void set_block_at(int n, uint8_t byte, shift_register_t *sr);

uint8_t block_shift(shift_register_t *sr);

extern testsuite shift_register_tests;
#endif
