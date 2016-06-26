#include <stdio.h>
#include "sbox.h"
#include "test.h"

int main(int argc, char *argv[])
{
  run_suite(sbox_tests);
  return 0;
}
