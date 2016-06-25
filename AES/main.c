#include <stdio.h>
#include "sbox.h"

int main(int argc, char *argv[])
{
  printf("S(0xc2) = %x\n", s_box(0xc2)); 
  return 0;
}
