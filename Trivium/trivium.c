/* trivium.c */

#include <stdlib.h>
#include <assert.h>

#include "trivium.h"
#include "shift_register.h"
#include "test.h"

trivium_keystream *new_keystream()
{
  trivium_keystream *ks;
  if ( (ks = (trivium_keystream *) malloc(sizeof(trivium_keystream))) == NULL)
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



/**        TESTS        **/

char allocate_and_delete()
{
  trivium_keystream *tks = new_keystream();
  char res = (tks != NULL);
  del_keystream(tks);
  return res;
}

test _trivium_tests[] = { {"Allocate and delete a Trivium keystream", allocate_and_delete } };

testsuite trivium_testsuite = { "trivium.c tests",
				_trivium_tests,
				sizeof(_trivium_tests) / sizeof(test) };
