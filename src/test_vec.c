#include <stdio.h>
#include <except/assert.h>
#include <mem.h>
#include "../include/ds.h"

void test_capacity_growth(void)
{

  
  VEC(int) vec = VEC_INIT(vec, 2);

  
  assert(VEC_CAPACITY(vec) == 2);
  assert(VEC_SIZE(vec) == 0);

  VEC_PUSH_BACK(vec, 2);
  VEC_PUSH_BACK(vec, 3);
  
  assert(VEC_CAPACITY(vec) == 4);

  VEC_PUSH_BACK(vec, 4);
  VEC_PUSH_BACK(vec, 5);
  
  assert(VEC_CAPACITY(vec) == 8);


  int x = VEC_POP_BACK(vec);
  x = VEC_POP_BACK(vec);
  assert(x == 4);
  assert(VEC_CAPACITY(vec) == 4);
  
  
  VEC_CLEAR(vec);
}

void test_basic_func(void)
{
  VEC(int) vec = VEC_INIT(vec, 100);
  assert(VEC_CAPACITY(vec) == 100, "Must be 100");
  assert(vec.buff != NULL, "Can't be null");
  
  assert(vec.size == 0, "Must be empty");
  
  VEC_PUSH_BACK(vec, 20);
  int b = 10;
  VEC_PUSH_BACK(vec, b);
  
  assert(vec.size == 2, "We should be able to store some values");

  int c = VEC_POP_BACK(vec);
  assert(c == 10, "Following the operations should 10");

  assert(20 == VEC_GET(vec, 0));

  VEC_INSERT(vec, 0, 1);
  VEC_INSERT(vec, 0, 2);
  VEC_INSERT(vec, 0, 3);
  VEC_INSERT(vec, 0, 4);

  assert(4 == VEC_GET(vec, 0));
  assert(3 == VEC_GET(vec, 1));
  assert(2 == VEC_GET(vec, 2));  
  
  VEC_CLEAR(vec);
}


int main(void)
{

  // test_basic_func();   
  test_capacity_growth();
  
  return 0;
}



