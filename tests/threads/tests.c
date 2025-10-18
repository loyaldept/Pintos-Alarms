#include "tests.h"

void test_name (const char *name)
{
  printf ("%s", name);
}

void fail (const char *format, ...)
{
  printf ("FAIL\n");
}

void pass (void)
{
  printf ("PASS\n");
}

