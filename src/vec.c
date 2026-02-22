#include <except/assert.h>
#include <stdio.h>
#include "../include/ds/vec.h"
#include "../include/ds/ds_mem.h"
#include "../include/ds/ds_exception.h"

void Vec_init(Vec *vec, uint32_t capacity, uint32_t type_size)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  
  vec->container.capacity = capacity;
  vec->container.size = 0;
  vec->container.type_size = type_size;
  
  vec->container.buff = ds_mem_alloc(capacity * type_size);
  if (vec->container.buff == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
}

void Vec_push_back(Vec *vec, uint8_t *new_val)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (new_val == NULL) RAISE(ExceptInvalidArgument, "NewVal can't be null");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");

  if (vec->container.size + 1 == vec->container.capacity) {
    /* TODO: This will growth in an exponential way */
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.type_size * vec->container.capacity * 2);
    if (vec->container.buff == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
    vec->container.capacity *= 2;
  }
  ds_mem_copy(vec->container.buff + vec->container.size * vec->container.type_size, new_val, vec->container.type_size);
  vec->container.size++;
}

uint8_t *Vec_pop_back(Vec *vec)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't pop an element in an empty vector");
  
  if (vec->container.capacity / 2 == vec->container.size + 1) {
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.type_size * vec->container.size + 1);
    if (vec->container.buff == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
    vec->container.capacity = vec->container.size + 1;
  }

  return vec->container.buff + (--vec->container.size) * vec->container.type_size;
}


uint8_t *Vec_get(Vec *vec, uint32_t index)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  if (vec->container.size <= index) RAISE(ExceptInvalidArgument, "Invalid index");
  
  return vec->container.buff + index * vec->container.type_size;
}

void Vec_insert(Vec *vec, uint32_t index, uint8_t *new_val)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  if (vec->container.size <= index) RAISE(ExceptInvalidArgument, "Invalid index");

  if (vec->container.size + 1 == vec->container.capacity) {
    /* TODO: This will growth in an exponential way */
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.type_size * vec->container.capacity * 2);
    if (vec->container.buff == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
    vec->container.capacity *= 2;
  }

  // TODO: See if it is possible to vectorize this part
  for (uint32_t i = vec->container.size; i > index; i--) {
    ds_mem_copy(vec->container.buff + i * vec->container.type_size, vec->container.buff + (i - 1) * vec->container.type_size, vec->container.type_size);
  }
  ds_mem_copy(vec->container.buff + index * vec->container.type_size, new_val, vec->container.type_size);
  vec->container.size++;
}

void Vec_remove(Vec *vec, uint32_t index)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  if (vec->container.size == 0) RAISE(ExceptInvalidArgument, "Can't pop an element in an empty vector");
  if (vec->container.size <= index) RAISE(ExceptInvalidArgument, "Invalid index");
  
  if (vec->container.capacity / 2 == vec->container.size + 1) {
    vec->container.buff = ds_mem_ralloc(vec->container.buff, vec->container.type_size * vec->container.size + 1);
    if (vec->container.buff == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
    vec->container.capacity = vec->container.size + 1;
  }

  // TODO: See if it is possible to vectorize this part
  for (uint32_t i = index; i < vec->container.size - 1; i++) {
    ds_mem_copy(vec->container.buff + i * vec->container.type_size,
	   vec->container.buff + (i + 1) * vec->container.type_size,
	   vec->container.type_size);
  }
  vec->container.size--;
}

void Vec_clear(Vec *vec)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  
  ds_mem_free(vec->container.buff);
  vec->container.buff = NULL;
  vec->container.type_size = 0;
  vec->container.capacity = 0;
  vec->container.size = 0;
}
