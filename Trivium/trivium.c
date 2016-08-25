/* trivium.c */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "trivium.h"
#include "shift_register.h"
#include "test.h"

/* get_and_byte gets the non-linear byte which is to be XORed
 * with the output byte. */
uint8_t get_and_byte(shift_register_t sr);

uint8_t reverse_byte(uint8_t b);

trivium_keystream *new_keystream()
{
  trivium_keystream *ks;
  if ( (ks = (trivium_keystream *) calloc(1, sizeof(trivium_keystream))) == NULL)
    return NULL;

  /* A:93,B:84,C:111 */
  if ((ks->srA = new_register(93)) == NULL) {
    free(ks);
    return NULL;
  }

  if ( (ks->srB = new_register(84)) == NULL ) {
    del_register(ks->srA);
    free(ks);
    return NULL;
  }

  if ( (ks->srC = new_register(111)) == NULL ) {
    del_register(ks->srB);
    del_register(ks->srA);
    free(ks);
    return NULL;
  }
  
  return ks;
}

void del_keystream(trivium_keystream *tks)
{
  assert( tks != NULL );
  assert( tks->srA != NULL );
  assert( tks->srB != NULL );
  assert( tks->srC != NULL );

  del_register(tks->srC);
  del_register(tks->srB);
  del_register(tks->srA);
  free(tks);
}

uint8_t get_and_byte(shift_register_t sr)
{
  uint8_t block1;
  uint8_t block2;

  block1 = get_block_at(1, sr);
  block2 = get_block_at(2, sr);

  return (block1 & block2);
}

uint8_t get_next_key(trivium_keystream *tks)
{
  /* Each shift register will produce a byte    *
   * output, which is fed into a) the overall   *
   * output byte, and b) into the next register *
   * (along with a feedback byte.               */
  uint8_t A_out, B_out, C_out;

  uint8_t feedback_A, feedback_B, feedback_C;
  feedback_A = get_block_at(24, *tks->srA);
  feedback_B = get_block_at(6, *tks->srB);
  feedback_C = get_block_at(24, *tks->srC);

  uint8_t feedforward_A, feedforward_B, feedforward_C;
  feedforward_A = get_block_at(27, *tks->srA);
  feedforward_B = get_block_at(15, *tks->srB);
  feedforward_C = get_block_at(45, *tks->srC);
  
  /* There are some values we need to save
   * before we shift. */
  uint8_t and_byte_A, and_byte_B, and_byte_C;
  and_byte_A = get_and_byte(*tks->srA);
  and_byte_B = get_and_byte(*tks->srB);
  and_byte_C = get_and_byte(*tks->srC);

  /* Do the shift and save the output bytes */
  uint8_t shift_byte_A, shift_byte_B, shift_byte_C;
  shift_byte_A = block_shift(tks->srA);
  shift_byte_B = block_shift(tks->srB);
  shift_byte_C = block_shift(tks->srC);

  /* Now compute the output of each shift register */
  A_out = (shift_byte_A ^ and_byte_A ^ feedforward_A);
  B_out = (shift_byte_B ^ and_byte_B ^ feedforward_B);
  C_out = (shift_byte_C ^ and_byte_C ^ feedforward_C);

  /* Feed the output of each shift register into the next, *
   * after XORing it with the feedback byte.               */
  set_block_at(76, A_out^feedback_B, tks->srB);
  set_block_at(103, B_out^feedback_C, tks->srC);
  set_block_at(85, C_out^feedback_A, tks->srA);
  
  return (A_out ^ B_out ^ C_out);
}

void load_key(char *key, char *iv, trivium_keystream *tks)
{
  int i;
  int KEY_BYTES = 10;

  /* We load the key in backwards using block_shift --- this
   * means we start at the end and copy in backwards, reversing
   * the bytes as we go. This complication is mostly due to the
   * fact that I decided to have the shift_registers outputting 
   * bytes oriented in the same way as an actual bytestring. */
  iv += KEY_BYTES - 1;
  key += KEY_BYTES - 1;
  
  /* Load the key and initialization vector into shift *
   * registers B and A respectively. */
  for (i = 0; i < KEY_BYTES; ++i, --iv, --key) {
    block_shift(tks->srA);
    block_shift(tks->srB);
    set_block_at(85, reverse_byte(*iv), tks->srA);
    set_block_at(76, reverse_byte(*key), tks->srB);
  }

  /* Set shift register C's bytes appropriately */
  set_block_at(0, 7, tks->srC);    
}

/* Neat byte reversing function, pulled from
 * stack overflow. Makes sense --- lesson: 
 * bytes aren't that big! Also, remember how
 * the bytes actually look in memory. */
uint8_t reverse_byte(uint8_t b)
{
  b = (b & 0x0F) << 4 | (b & 0xF0) >> 4;
  b = (b & 0x33) << 2 | (b & 0xCC) >> 2;
  b = (b & 0x55) << 1 | (b & 0xAA) >> 1;
  return b;
}


trivium_keystream *get_primed_trivium_cipher(char *key, char *iv)
{
  trivium_keystream *tks;
  int i;
  
  if ( (tks = new_keystream()) == NULL)
    return NULL;

  load_key(key, iv, tks);

  /* Now prime the system */
  for(i = 0; i<1152 ; ++i)
    get_next_key(tks);

  return tks;
}

 uint8_t encrypt(uint8_t byte, uint8_t key)
{
  return (byte ^ key);
}

uint8_t decrypt(uint8_t byte, uint8_t key)
{
  /* encryption is self-inverse */
  return encrypt(byte, key);
}

/**        TESTS        **/

char allocate_and_delete()
{
  trivium_keystream *tks = new_keystream();
  char res = (tks != NULL);
  del_keystream(tks);
  return res;
}

char check_reverse_byte()
{
  uint8_t test = 0b11010111;
  return (reverse_byte(test) == 0b11101011);
}

char encrypt_decrypt()
{
  uint8_t byte, key;
  byte = 't';
  key = 'k';
  return ('t' == decrypt(encrypt(byte, key),key));
}

/* This is mostly to check the cipher runs */
char trivium_encrypt_string()
{
  char *string = "Test string";
  char *key = "keystring1";
  char *iv = "initialvec";
  uint8_t ks;

  trivium_keystream *enc;
  
  enc = get_primed_trivium_cipher(key, iv);

  do {
    ks = get_next_key(enc);
    printf("%x ", encrypt(*string++, ks));
  } while (*string != '\0');
  putchar('\n');

  del_keystream(enc);
  return 1;
}

test _trivium_tests[] = {
    {"Allocate and delete a Trivium keystream", allocate_and_delete },
    {"Check reverse_byte reverses bytes", check_reverse_byte },
    {"Check decrypt is inverse to encrypt", encrypt_decrypt},
    {"Encrypt \"Test String\"", trivium_encrypt_string} };

testsuite trivium_testsuite = { "trivium.c tests",
				_trivium_tests,
				sizeof(_trivium_tests) / sizeof(test) };
