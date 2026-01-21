/* TODO: Add the exceptions module */
#include <except/assert.h>
#include <string.h>
#include "../include/ds/vec.h"

void Vec_init(Vec *vec, uint32_t capacity, uint32_t type_size)
{
  assert(vec != NULL, "vec can't be null");
  assert(capacity > 0, "Can't be zero");
  assert(type_size > 0, "Can't be zero");
  
  vec->container.capacity = capacity;
  vec->container.size = 0;
  vec->container.type_size = type_size;
  
  vec->container.buff = ds_mem_alloc(capacity * type_size);
  
  assert(vec->container.buff != NULL, "Can't be null");
}

void Vec_push_back(Vec *vec, uint8_t *new_val)
{
  assert(vec != NULL, "vec can't be null");
  assert(new_val != NULL, "vec can't be null");
  assert(vec->container.capacity > 0, "Can't be zero");
  assert(vec->container.type_size > 0, "Can't be zero");
  assert(vec->container.buff != NULL, "Can't be null");

  if (vec->container.size + 1 == vec->container.capacity) {
    /* TODO: This will growth in an exponential way */
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.capacity * 2);
  }
  ds_mem_copy(vec->container.buff + vec->container.size * vec->container.type_size, new_val, vec->container.type_size);
  vec->container.size++;
}

uint8_t *Vec_pop_back(Vec *vec)
{
  assert(vec != NULL, "vec can't be null");
  assert(vec->container.capacity > 0, "Can't be zero");
  assert(vec->container.type_size > 0, "Can't be zero");
  assert(vec->container.size > 0, "Can't pop an element in a empty vector");
  assert(vec->container.buff != NULL, "Can't be null");
  
  /* TODO: test this line */
  if (vec->container.capacity / 2 == vec->container.size + 1) {
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.size + 1);
  }

  return vec->container.buff + (--vec->container.size) * vec->container.type_size;
}


uint8_t *Vec_get(Vec *vec, uint32_t index)
{
  assert(vec != NULL, "vec can't be null");
  assert(vec->container.capacity > 0, "Can't be zero");
  assert(vec->container.type_size > 0, "Can't be zero");
  assert(vec->container.size > index, "Incorrect index");
  assert(vec->container.buff != NULL, "Can't be null");
  
  return vec->container.buff + index * vec->container.type_size;
}

void Vec_insert(Vec *vec, uint32_t index, uint8_t *new_val)
{
  assert(vec != NULL, "vec can't be null");
  assert(new_val != NULL, "vec can't be null");
  assert(vec->container.capacity > 0, "Can't be zero");
  assert(vec->container.type_size > 0, "Can't be zero");
  assert(vec->container.size > index, "Incorrect index");
  assert(vec->container.buff != NULL, "Can't be null");

  if (vec->container.size + 1 == vec->container.capacity) {
    /* TODO: This will growth in an exponential way */
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.capacity * 2);
  }

  for (uint32_t i = vec->container.size; i > index; i--) {
    ds_mem_copy(vec->container.buff + i * vec->container.type_size, vec->container.buff + (i - 1) * vec->container.type_size, vec->container.type_size);
  }
  ds_mem_copy(vec->container.buff + index * vec->container.type_size, new_val, vec->container.type_size);

  vec->container.size++;
}

void Vec_remove(Vec *vec, uint32_t index)
{
  assert(vec != NULL, "vec can't be null");
  assert(vec->container.capacity > 0, "Can't be zero");
  assert(vec->container.type_size > 0, "Can't be zero");
  assert(vec->container.size > 0, "Can't pop an element in a empty vector");
  assert(vec->container.size > index, "Incorrect index");
  assert(vec->container.buff != NULL, "Can't be null");
  
  if (vec->container.capacity / 2 == vec->container.size + 1) {
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.size + 1);
  }
  
  for (uint32_t i = index; i < vec->container.size - 1; i++) {
    ds_mem_copy(vec->container.buff + i * vec->container.type_size,
	   vec->container.buff + (i + 1) * vec->container.type_size,
	   vec->container.type_size);
  }
  vec->container.size--;
}

void Vec_clear(Vec *vec)
{
  assert(vec != NULL, "vec can't be null");
  assert(vec->container.capacity > 0, "Can't be zero");
  assert(vec->container.type_size > 0, "Can't be zero");
  assert(vec->container.buff != NULL, "Can't be null");
  
  ds_mem_free(vec->container.buff);
  vec->container.buff = NULL;
  vec->container.type_size = 0;
  vec->container.capacity = 0;
  vec->container.size = 0;
}
