#include <except/assert.h>
#include <stdio.h>

#include "../include/ds/consts.h"
#include "../include/ds/vec.h"
#include "../include/ds/ds_mem.h"
#include "../include/ds/ds_exception.h"


// Include the private implemenations of the linked list
#define ITERATOR_INTERNAL
#undef ITERATOR_INCLUDED
#include "../include/ds/iterator.h"

struct Vec {
  Container container;
  DS_State state;
};

static void Vec_set_state(Vec *vec, DS_State state)
{
  assert(vec != NULL);
  assert(state == DS_STATE_ALIVE || state == DS_STATE_DESTROYED);
  vec->state = state;
}

static DS_State Vec_get_state(Vec *vec)
{
  assert(vec != NULL);
  return vec->state;
}


static void Vec_basic_validations(Vec *vec)
{
  if (vec == NULL) RAISE(ExceptInvalidArgument, "Vec can't be null");
  if (Vec_get_state(vec) == DS_STATE_DESTROYED) RAISE(ExceptDestroyedDataStructure, "Vector has been destroyed");
}

void Vec_init(Vec *vec, uint32_t capacity, uint32_t type_size)
{
  Vec_basic_validations(vec);
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
  Vec_basic_validations(vec);
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
  Vec_basic_validations(vec);
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
  Vec_basic_validations(vec);
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  if (vec->container.size <= index) RAISE(ExceptInvalidIndex);
  
  return vec->container.buff + index * vec->container.type_size;
}

void Vec_insert(Vec *vec, uint32_t index, uint8_t *new_val)
{
  Vec_basic_validations(vec);
  if (vec->container.size <= index) RAISE(ExceptInvalidArgument, "Invalid index");
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");

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
  Vec_basic_validations(vec);
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  if (vec->container.size == 0) RAISE(ExceptInvalidArgument, "Can't pop an element in an empty vector");
  if (vec->container.size <= index) RAISE(ExceptInvalidIndex);
  
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
  Vec_basic_validations(vec);
  if (vec->container.buff == NULL) RAISE(ExceptInvalidArgument, "Container.Buff can't be null");
  if (vec->container.capacity == 0) RAISE(ExceptInvalidArgument, "Capacity can't be zero");
  if (vec->container.type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");

  vec->container.buff = ds_mem_ralloc(vec->container.buff, DEFAULT_VEC_CAPACITY);
  vec->container.capacity = DEFAULT_VEC_CAPACITY;
  vec->container.size = 0;
}


void Vec_destroy(Vec *vec)
{
  Vec_basic_validations(vec);
  
  ds_mem_free(vec->container.buff);
  vec->container.buff = NULL;
  vec->container.size = vec->container.type_size = vec->container.capacity = 0;
  
  Vec_set_state(vec, DS_STATE_DESTROYED);
}


static void Vec_Iterator_inc(Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  Vec *vec = (Vec *) iterator->ds;

  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty vector");
  if (iterator->index > vec->container.size) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  if (iterator->index == vec->container.size) RAISE(ExceptInvalidArgument, "Can't iterate; already pass the top");
  if (iterator->index_ptr == NULL) RAISE(ExceptInvalidArgument, "Invalid Iterator");

  iterator->index++;
  iterator->index_ptr = (uint8_t *) (vec->container.buff + vec->container.type_size * iterator->index);
}

static void Vec_Iterator_dec(Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  Vec *vec = (Vec *) iterator->ds;
  
  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty vector");
  if (iterator->index > vec->container.size) RAISE(ExceptInvalidArgument, "Can't iterate; already pass the bottom");
  if (iterator->index_ptr == NULL) RAISE(ExceptInvalidArgument, "Invalid Iterator");

  iterator->index--;
  iterator->index_ptr = (uint8_t *) (vec->container.buff + vec->container.type_size * iterator->index);
}

static const uint8_t *Vec_Iterator_get_data(Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  Vec *vec = (Vec *) iterator->ds;

  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty vector");
  // We must to return NULL if we have some null iterator
  if (iterator->index >= vec->container.size)
    return NULL;

  return vec->container.buff + (vec->container.type_size * iterator->index);
}

static void Vec_Iterator_move_at(Iterator *iterator, uint32_t index)
{
  Iterator_basic_validations(iterator);
  Vec *vec = (Vec *) iterator->ds;
  
  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty vector");
  if (iterator->index >= vec->container.size) RAISE(ExceptInvalidIndex, "Index out of bounds");

  if (index > vec->container.size / 2) {
    Vec_begin(vec, iterator, iterator->cmp);
    for (uint32_t i = vec->container.size - 1; i > index; i--)
      Vec_Iterator_dec(iterator);
  } else {
    Vec_end(vec, iterator, iterator->cmp);
    for (uint32_t i = 0; i < index; i++)
      Vec_Iterator_inc(iterator);
  }
}

void Vec_begin(Vec *vec, Iterator *iterator, int (*const cmp)(Iterator *, Iterator *))
{
  Vec_basic_validations(vec);
  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch any iterator from an empty vector");

  assert(vec->container.buff != NULL);
  
  Iterator_init(
    iterator,
    (const uint8_t *) vec,
    vec->container.buff,
    0,
    &Vec_Iterator_inc,
    &Vec_Iterator_dec,
    &Vec_Iterator_get_data,
    &Vec_Iterator_move_at,
    cmp
  );
}


void Vec_end(Vec *vec, Iterator *iterator, int (*const cmp)(Iterator *, Iterator *))
{
  Vec_basic_validations(vec);
  if (vec->container.size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch any iterator from an empty vector");

  assert(vec->container.buff != NULL);
  
  Iterator_init(
    iterator,
    (const uint8_t *) vec,
    vec->container.buff + vec->container.type_size * (vec->container.size - 1),
    vec->container.size - 1,
    &Vec_Iterator_inc,
    &Vec_Iterator_dec,
    &Vec_Iterator_get_data,
    &Vec_Iterator_move_at,
    cmp
  );
}
