/* sbox.h --- definitions for the substitution box phase *
 * of AES */

#ifndef _SBOX_H
#define _SBOX_H

#include <stdint.h>
#include "test.h"

uint8_t s_box(uint8_t byte);
uint8_t s_box_inverse(uint8_t byte);

extern testsuite sbox_tests;

#endif
