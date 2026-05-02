#include <string.h>
#include <except/assert.h>

#include "../include/ds/ds_exception.h"

#define ITERATOR_INTERNAL
#include "../include/ds/iterator.h"


void Iterator_basic_validations(Iterator *iterator)
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (iterator->ds == NULL) RAISE(ExceptUninitializedIterator, "Data structure pointer (ds) can't be NULL");
}

// Iterator_init: We can handle null compare functions
void Iterator_init(
  Iterator        *iterator,
  const uint8_t   *ds,
  uint8_t   *index_ptr,
  uint32_t         index,
  void           (* const inc)     (Iterator *),
  void           (* const dec)     (Iterator *),
  const uint8_t *(* const get_data)(Iterator *),
  void           (* const move_at) (Iterator *, uint32_t),
  int            (* const cmp)     (Iterator *, Iterator *)
)
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (ds == NULL) RAISE(ExceptUninitializedIterator, "Data structure pointer (ds) can't be NULL");
  if (inc == NULL) RAISE(ExceptInvalidArgument, "Function pointer 'inc' can't be NULL");
  if (dec == NULL) RAISE(ExceptInvalidArgument, "Function pointer 'dec' can't be NULL");
  if (get_data == NULL) RAISE(ExceptInvalidArgument, "Function pointer 'get_data' can't be NULL");
  if (move_at == NULL) RAISE(ExceptInvalidArgument, "Function pointer 'move_at' can't be NULL");

  iterator->ds        = ds;
  iterator->index_ptr = index_ptr;
  iterator->index     = index;
  iterator->inc       = inc;
  iterator->dec       = dec;
  iterator->get_data  = get_data;
  iterator->move_at   = move_at;
  iterator->cmp       = cmp;
}


void Iterator_set_cmp(Iterator *iterator, int (* const cmp)(Iterator *, Iterator *))
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (cmp == NULL) RAISE(ExceptInvalidArgument, "Function pointer 'cmp' can't be NULL");
  iterator->cmp = cmp;
}

static void validate_pair(Iterator *a, Iterator *b)
{
  Iterator_basic_validations(a);
  Iterator_basic_validations(b);
  
  if (a == NULL || b == NULL)
    RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (a->get_data == NULL || b->get_data == NULL)
    RAISE(ExceptUninitializedIterator, "Iterator has not been initialized");
}

/* Null return from get_data means exhausted/sentinel iterator */
int Iterator_cmp_int(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const int va = *(const int *) da;
  const int vb = *(const int *) db;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_long(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const long va = *(const long *) da;
  const long vb = *(const long *) db;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_short(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const short va = *(const short *) da;
  const short vb = *(const short *) db;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_uint(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const unsigned int va = *(const unsigned int *) da;
  const unsigned int vb = *(const unsigned int *) db;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_ulong(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const unsigned long va = *(const unsigned long *) da;
  const unsigned long vb = *(const unsigned long *) db;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_ushort(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const unsigned short va = *(const unsigned short *) da;
  const unsigned short vb = *(const unsigned short *) db;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_str(Iterator *a, Iterator *b)
{
  validate_pair(a, b);
  const uint8_t *da = a->get_data(a);
  const uint8_t *db = b->get_data(b);
  if (da == NULL && db == NULL) return 0;
  if (da == NULL) return 1;
  if (db == NULL) return -1;
  const char *va = *(const char **) da;
  const char *vb = *(const char **) db;
  if (va == NULL || vb == NULL)
    RAISE(ExceptInvalidArgument, "String pointer inside iterator can't be NULL");
  return strcmp(va, vb);
}

int Iterator_cmp_raise_except(Iterator *a, Iterator *b)
{
  (void) a;
  (void) b;
  RAISE(ExceptInvalidArgument,
    "No compare function defined for this iterator type — "
    "provide an explicit cmp or specialize _Generic for your type");
  return 0;
}
