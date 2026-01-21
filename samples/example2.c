#include <stdio.h>
#include <stdint.h>

typedef struct {
  size_t type_size, size, capacity;
  uint8_t *buff;
} Vector;


#define VEC_INIT(type, ...)						\
  {.type_size = sizeof(type) __VA_OPT__(, .capacity = __VA_ARGS__)}

void Vec_push_back(Vector *vec, uint8_t *element, size_t element_size)
{
	
}

#define VEC_PUSH_BACK(vec, element)					\
  {									\
    typeof(element) __ds_element = element;				\
    Vec_push_back(&(vec), (uint8_t *) &__ds_element, (vec).type_size);	\
  }

int main(void)
{
  Vector veci = VEC_INIT(int, 10);
  printf("%zu\n", veci.capacity);

  VEC_PUSH_BACK(veci, 10);
  int var = 10;
  VEC_PUSH_BACK(veci, var);
}


