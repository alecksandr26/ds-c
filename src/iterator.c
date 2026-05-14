#include <string.h>
#include <except/assert.h>

#include "../include/ds/ds_exception.h"

#define ITERATOR_PRIVATE
#include "../include/ds/iterator.h"


void Iterator_basic_validations(const Iterator *iterator)
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (iterator->ds == NULL) RAISE(ExceptUninitializedIterator, "Data structure pointer (ds) can't be NULL");
}

void Iterator_init(
  Iterator        *iterator,
  const uint8_t   *ds,
  uint8_t         *index_ptr,
  uint32_t         index,
  void           (* const inc)     (Iterator *),
  void           (* const dec)     (Iterator *),
  const uint8_t *(* const get_data)(const Iterator *),
  void           (* const move_at) (Iterator *, uint32_t),
  void           (* const insert)  (Iterator *, const uint8_t *),
  void           (* const remove)  (Iterator *),
  void           (* const replace) (Iterator *, const uint8_t *),
  int            (* const cmp)     (const uint8_t *, const uint8_t *)
)
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (ds == NULL)       RAISE(ExceptInvalidArgument, "Data structure pointer (ds) can't be NULL");
  if (inc == NULL)      RAISE(ExceptInvalidArgument, "Function pointer 'inc' can't be NULL");
  if (dec == NULL)      RAISE(ExceptInvalidArgument, "Function pointer 'dec' can't be NULL");
  if (get_data == NULL) RAISE(ExceptInvalidArgument, "Function pointer 'get_data' can't be NULL");
  if (move_at == NULL)  RAISE(ExceptInvalidArgument, "Function pointer 'move_at' can't be NULL");
  if (insert == NULL)   RAISE(ExceptInvalidArgument, "Function pointer 'insert' can't be NULL");
  if (remove == NULL)   RAISE(ExceptInvalidArgument, "Function pointer 'remove' can't be NULL");
  if (replace == NULL)  RAISE(ExceptInvalidArgument, "Function pointer 'replace' can't be NULL");
  if (cmp == NULL)      RAISE(ExceptInvalidArgument, "Function pointer 'cmp' can't be NULL");

  iterator->ds        = ds;
  iterator->index_ptr = index_ptr;
  iterator->index     = index;
  iterator->inc       = inc;
  iterator->dec       = dec;
  iterator->get_data  = get_data;
  iterator->move_at   = move_at;
  iterator->insert    = insert;
  iterator->remove    = remove;
  iterator->replace   = replace;
  iterator->cmp       = cmp;
}


void Iterator_set_cmp(Iterator *iterator, int (* const cmp)(const uint8_t *, const uint8_t *))
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator pointer can't be NULL");
  if (cmp == NULL)      RAISE(ExceptInvalidArgument, "Function pointer 'cmp' can't be NULL");
  iterator->cmp = cmp;
}


/* ------------------------------------------------------------------ *
 *  Primitive comparison functions                                      *
 *  Signature: int cmp(const uint8_t *a, const uint8_t *b)             *
 *  NULL  pointer → treated as a sentinel / exhausted iterator value:  *
 *    both NULL → equal (0), left NULL → greater (1), right NULL → less (-1)
 * ------------------------------------------------------------------ */

int Iterator_cmp_int(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const int va = *(const int *) a;
  const int vb = *(const int *) b;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_long(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const long va = *(const long *) a;
  const long vb = *(const long *) b;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_short(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const short va = *(const short *) a;
  const short vb = *(const short *) b;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_uint(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const unsigned int va = *(const unsigned int *) a;
  const unsigned int vb = *(const unsigned int *) b;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_ulong(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const unsigned long va = *(const unsigned long *) a;
  const unsigned long vb = *(const unsigned long *) b;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_ushort(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const unsigned short va = *(const unsigned short *) a;
  const unsigned short vb = *(const unsigned short *) b;
  return (va > vb) - (va < vb);
}

int Iterator_cmp_str(const uint8_t *a, const uint8_t *b)
{
  if (a == NULL && b == NULL) return  0;
  if (a == NULL)              return  1;
  if (b == NULL)              return -1;
  const char *va = *(const char **) a;
  const char *vb = *(const char **) b;
  if (va == NULL || vb == NULL)
    RAISE(ExceptInvalidArgument, "String pointer inside iterator can't be NULL");
  return strcmp(va, vb);
}

int Iterator_cmp_raise_except(const uint8_t *a, const uint8_t *b)
{
  (void) a;
  (void) b;
  RAISE(ExceptInvalidArgument,
    "No compare function defined for this iterator type — "
    "provide an explicit cmp or specialize _Generic for your type");
  return 0;
}
