#ifndef DS_CONTAINER_INCLUDED
#define DS_CONTAINER_INCLUDED

#include <inttypes.h>

typedef struct {
  uint8_t *buff;
  uint32_t capacity, size, type_size;
} Container;


#endif
