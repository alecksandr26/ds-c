#include <stdio.h>
#include <except/assert.h>
#include <except.h>

#include "../include/ds/vec.h"
#include "../include/ds/linked.h"
#include "../include/ds/sort.h"
#include "../include/ds/ds_exception.h"

/* ── Helpers ─────────────────────────────────────────────────────────────── */

static void vec_fill(VEC(int) *vec, int *arr, uint32_t n)
{
  for (uint32_t i = 0; i < n; i++)
    VEC_PUSH_BACK(*vec, arr[i]);
}

static void vec_assert_sorted_asc(VEC(int) *vec)
{
  for (uint32_t i = 0; i < vec->size - 1; i++)
    assert(VEC_GET(*vec, i) <= VEC_GET(*vec, i + 1));
}

static void linked_fill(LINKED_LIST(int) *list, int *arr, uint32_t n)
{
  for (uint32_t i = 0; i < n; i++)
    LINKED_LIST_PUSH_BACK(*list, arr[i]);
}

/* ── Vector Tests ────────────────────────────────────────────────────────── */

static void test_vec_sorts()
{
  int data[] = {50, 10, 40, 20, 30};
    
  // Prueba con comparador explícito (Nueva sintaxis sin .size)
  VEC(int) v1 = VEC_INIT(v1);
  vec_fill(&v1, data, 5);
  QUICK_SORT(v1, Iterator_cmp_int); 
  vec_assert_sorted_asc(&v1);
    
  // Prueba con comparador automático (Sintaxis simplificada)
  VEC(int) v2 = VEC_INIT(v2);
  vec_fill(&v2, data, 5);
  MERGE_SORT(v2); 
  vec_assert_sorted_asc(&v2);

  VEC_DESTROY(v1); VEC_DESTROY(v2);
}

/* ── LinkedList Tests ────────────────────────────────────────────────────── */

static void test_linked_sorts()
{
  int data[] = {5, 1, 4, 2, 3};
    
  LINKED_LIST(int) l = LINKED_LIST_INIT(l);
  
  
  linked_fill(&l, data, 5);
    
  QUICK_SORT(l);

  ITERATOR(int) it = LINKED_LIST_BEGIN(l, it);
  ITERATOR(int) end = LINKED_LIST_END(l, it);

  int prev = ITERATOR_GET_DATA(it);

  ITERATOR_INC(it);

  while (ITERATOR_CMP(end, it) >= 0) {
    assert(prev < ITERATOR_GET_DATA(it));
    prev = ITERATOR_GET_DATA(it);
    ITERATOR_INC(it);
  }
    
  assert(LINKED_LIST_SIZE(l) == 5);
  assert(LINKED_LIST_FRONT(l) == 1);

  LINKED_LIST_CLEAR(l);

  linked_fill(&l, data, 5);

  MERGE_SORT(l);

  LINKED_LIST_BEGIN(l, it);
  LINKED_LIST_END(l, it);

  prev = ITERATOR_GET_DATA(it);

  ITERATOR_INC(it);

  while (ITERATOR_CMP(end, it) >= 0) {
    assert(prev < ITERATOR_GET_DATA(it));
    prev = ITERATOR_GET_DATA(it);
    ITERATOR_INC(it);
  }
  
  assert(LINKED_LIST_SIZE(l) == 5);
  assert(LINKED_LIST_FRONT(l) == 1);
    
  LINKED_LIST_DESTROY(l);
}

static void test_sort_exceptions()
{

  // 1. Test NULL pointer
  TRY
    Vec_quick_sort(NULL, Iterator_cmp_int);
  ELSE
    // En tu except.h, la info global está en Except_raise_info
    assert(Except_raise_info.reason == ExceptInvalidArgument.reason);
  END_TRY;

  // 2. Test NULL comparison function
  VEC(int) v = VEC_INIT(v);
  TRY
    QUICK_SORT(v, NULL);
  ELSE
    assert(Except_raise_info.reason == ExceptNullCmpFunction.reason);
  END_TRY;

  // 3. Test Destroyed Data Structure
  VEC_DESTROY(v);
  TRY
    QUICK_SORT(v);
  ELSE
    assert(Except_raise_info.reason == ExceptDestroyedDataStructure.reason);
  END_TRY;
}

/* ── Entry Point ─────────────────────────────────────────────────────────── */

int main(void)
{
  test_vec_sorts();
  test_linked_sorts();
  test_sort_exceptions();
  
  return 0;
}
