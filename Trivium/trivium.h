/* trivium.h */
/* Header file for functions pertaining to an implementation
   of the trivium cypher */

#ifndef _TRIVIUM_H
#define _TRIVIUM_H

#include "shift_register.h"

typedef struct _trivium_ks {
  shift_register_t *srA;
  shift_register_t *srB;
  shift_register_t *srC;
} trivium_keystream;

trivium_keystream *new_keystream();
void del_keystream(trivium_keystream *tks);

uint8_t get_next_key(trivium_keystream *tks);

/* load_key expects to receive an 80-bit key, and an 80-bit
 * initialization vector, which are loaded into the shift
 * registers. The third shift register is also initialized. */
void load_key(char *key, char *iv, trivium_keystream *tks);

extern testsuite trivium_testsuite;

#endif
