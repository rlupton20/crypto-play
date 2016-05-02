/* trivium.c */

#include <stdlib.h>

#include "trivium.h"
#include "shift_register.h"

trivium_keystream *new_keystream()
{
  trivium_keystream *ks = malloc(sizeof(trivium_keystream));
  /* ADD ERROR CHECKING */
  /* A:93,B:84,C:111 */
  ks->srA = new_register();
  return ks;
}
