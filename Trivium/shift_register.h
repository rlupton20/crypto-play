/* shift_register.h */
/* Contains basic functions for dealing with long blocks of bytes
   which are intended for use as a shift register */

#ifndef _SHIFTREG
#define _SHIFTREG

typedef unsigned char block_t;

typedef struct s_shift_register {
  int nbits;
  block_t *reg;
} shift_register_t;

shift_register_t *new_register(int nbytes);
void del_register(shift_register_t *sr);

char get_bit(int n, shift_register_t sr);
void set_bit(int n, shift_register_t *sr);

block_t get_block_at(int n, shift_register_t sr);

#endif
