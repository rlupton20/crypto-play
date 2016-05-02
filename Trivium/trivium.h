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

#endif
