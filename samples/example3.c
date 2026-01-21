#include <stdio.h>
#include <stddef.h>


#define DEF_VEC(type)				\
  typedef struct {				\
    type *buff;					\
    size_t capacity, size;			\
  } ___DEF_VEC__ ## type ## __

#define VEC(type)				\
  ___DEF_VEC__ ## type ## __

int main(void)
{
  DEF_VEC(int);
	
  VEC(int) myvec;

  typedef int * int_ptr;
  DEF_VEC(int_ptr);
  VEC(int_ptr) myvec2;
	
  return 0;
}

