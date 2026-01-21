#include <except/assert.h>
#include "../include/ds.h"

int main(void)
{
  void *ptr = ds_mem_alloc(20);
  assert(ptr != NULL, "ptr should not be a pointer");
  ds_mem_free(ptr);
  
  return 0;
}




