#include "../include/ds/sort.h"

#undef ITERATOR_INCLUDED
#define ITERATOR_PRIVATE
#include "../include/ds/iterator.h"

#undef VEC_INCLUDED
#define VEC_PRIVATE
#include "../include/ds/vec.h"

#undef LINKED_LIST_INCLUDED
#define LINKED_LIST_PRIVATE
#include "../include/ds/linked.h"

#include "../include/ds/ds_exception.h"
#include "../include/ds/ds_mem.h"

/* ══════════════════════════════════════════════════════════════════════════
 * Internal Helpers & Validations
 * ══════════════════════════════════════════════════════════════════════════ */

static void Sort_basic_validations(const void *ptr, int (*cmp)(const uint8_t *, const uint8_t *))
{
  if (ptr == NULL)
    RAISE(ExceptInvalidArgument, "Sort: Container/Array pointer cannot be NULL");
  if (cmp == NULL)
    RAISE(ExceptNullCmpFunction, "Sort: Comparison function cannot be NULL");
}

static void swap_elements(uint8_t *a, uint8_t *b, uint32_t size)
{
  uint8_t tmp[size];
  ds_mem_copy(tmp, a, size);
  ds_mem_copy(a, b, size);
  ds_mem_copy(b, tmp, size);
}

/* ══════════════════════════════════════════════════════════════════════════
 * Array Logic (Used by Vec and Raw Arrays)
 * ══════════════════════════════════════════════════════════════════════════ */

static uint32_t array_partition(
  uint8_t *arr, uint32_t low, uint32_t high, uint32_t ts,
  int (*cmp)(const uint8_t *, const uint8_t *))
{
  uint8_t *pivot = arr + high * ts;
  uint32_t i = low;
  for (uint32_t j = low; j < high; j++) {
    if (cmp(arr + j * ts, pivot) <= 0) {
      swap_elements(arr + i * ts, arr + j * ts, ts);
      i++;
    }
  }
  swap_elements(arr + i * ts, pivot, ts);
  return i;
}

static void array_quick_sort_rec(
  uint8_t *arr, uint32_t low, uint32_t high, uint32_t ts,
  int (*cmp)(const uint8_t *, const uint8_t *))
{
  if (low < high) {
    uint32_t pi = array_partition(arr, low, high, ts, cmp);
    if (pi > 0) array_quick_sort_rec(arr, low, pi - 1, ts, cmp);
    array_quick_sort_rec(arr, pi + 1, high, ts, cmp);
  }
}

static void array_merge_sort_rec(
  uint8_t *arr, uint32_t l, uint32_t r, uint32_t ts,
  int (*cmp)(const uint8_t *, const uint8_t *))
{
  if (l < r) {
    uint32_t m = l + (r - l) / 2;
    array_merge_sort_rec(arr, l, m, ts, cmp);
    array_merge_sort_rec(arr, m + 1, r, ts, cmp);

    uint32_t n1 = m - l + 1, n2 = r - m;
    uint8_t *L = ds_mem_alloc(n1 * ts), *R = ds_mem_alloc(n2 * ts);
    ds_mem_copy(L, arr + l * ts, n1 * ts);
    ds_mem_copy(R, arr + (m + 1) * ts, n2 * ts);

    uint32_t i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
      if (cmp(L + i * ts, R + j * ts) <= 0)
        ds_mem_copy(arr + (k++) * ts, L + (i++) * ts, ts);
      else
        ds_mem_copy(arr + (k++) * ts, R + (j++) * ts, ts);
    }
    while (i < n1) ds_mem_copy(arr + (k++) * ts, L + (i++) * ts, ts);
    while (j < n2) ds_mem_copy(arr + (k++) * ts, R + (j++) * ts, ts);
    ds_mem_free(L);
    ds_mem_free(R);
  }
}



/* Partition using last node (head) as pivot.
 * Returns the node that ended up in the pivot's final position. */
static Node *ll_partition(
  Node *low, Node *high,
  int (*cmp)(const uint8_t *, const uint8_t *))
{
  /* Pivot is the data at high (last node in range) */
  const uint8_t *pivot = high->data_ptr;

  /* i trails behind: last node whose data is <= pivot */
  Node *i = low->prev; /* NULL means "nothing placed yet" */

  for (Node *j = low; j != high; j = j->next) {
    if (cmp(j->data_ptr, pivot) <= 0) {
      /* Advance i */
      i = (i == NULL) ? low : i->next;
      /* Swap data between i and j */
      const uint8_t *tmp = i->data_ptr;
      i->data_ptr        = j->data_ptr;
      j->data_ptr        = tmp;
    }
  }

  /* Place pivot in its correct position */
  i = (i == NULL) ? low : i->next;
  const uint8_t *tmp = i->data_ptr;
  i->data_ptr        = high->data_ptr;
  high->data_ptr     = tmp;

  return i;
}

static void ll_quick_sort_rec(
  Node *low, Node *high,
  int (*cmp)(const uint8_t *, const uint8_t *),
  uint32_t type_size)
{
  if (high == NULL || low == NULL || low == high || low == high->next)
    return;

  Node *pivot = ll_partition(low, high, cmp);

  /* Sort left partition (tail side: prev direction) */
  ll_quick_sort_rec(low, pivot->prev, cmp, type_size);

  /* Sort right partition (head side: next direction) */
  ll_quick_sort_rec(pivot->next, high, cmp, type_size);
}



/* Split the list starting at `head` into two halves.
 * Uses tortoise & hare to find the midpoint.
 * After the call, the first half ends at slow, and slow->next is severed.
 * Returns the start of the second half. */
static Node *ll_split(Node *head)
{
  Node *slow = head;
  Node *fast = head->next;

  while (fast != NULL && fast->next != NULL) {
    slow = slow->next;
    fast = fast->next->next;
  }

  /* Sever the two halves */
  Node *second = slow->next;
  slow->next = NULL;
  if (second != NULL)
    second->prev = NULL;

  return second;
}

/* Merge two sorted sublists into one sorted list.
 * Returns the new head (first logical = smallest element). */
static Node *ll_merge(Node *a, Node *b,
  int (*cmp)(const uint8_t *, const uint8_t *))
{
  if (a == NULL) return b;
  if (b == NULL) return a;

  Node *result;

  if (cmp(a->data_ptr, b->data_ptr) <= 0) {
    result       = a;
    result->next = ll_merge(a->next, b, cmp);
  } else {
    result       = b;
    result->next = ll_merge(a, b->next, cmp);
  }

  if (result->next != NULL)
    result->next->prev = result;
  result->prev = NULL;

  return result;
}

/* Recursive merge sort on raw node chain.
 * Returns the new first node of the sorted sublist. */
static Node *ll_merge_sort_rec(Node *head,
  int (*cmp)(const uint8_t *, const uint8_t *))
{
  if (head == NULL || head->next == NULL)
    return head;

  Node *second = ll_split(head);

  head   = ll_merge_sort_rec(head,   cmp);
  second = ll_merge_sort_rec(second, cmp);

  return ll_merge(head, second, cmp);
}


/* ══════════════════════════════════════════════════════════════════════════
 * Public API
 * ══════════════════════════════════════════════════════════════════════════ */

void Vec_quick_sort(Vec *vec, int (*cmp)(const uint8_t *, const uint8_t *))
{
  Sort_basic_validations(vec, cmp);
  if (vec->state == DS_STATE_DESTROYED)
    RAISE(ExceptDestroyedDataStructure, "Vec_quick_sort: Vector is destroyed");
  if (vec->container.size <= 1) return;

  array_quick_sort_rec(vec->container.buff, 0, vec->container.size - 1, vec->container.type_size, cmp);
}

void Vec_merge_sort(Vec *vec, int (*cmp)(const uint8_t *, const uint8_t *))
{
  Sort_basic_validations(vec, cmp);
  if (vec->state == DS_STATE_DESTROYED)
    RAISE(ExceptDestroyedDataStructure, "Vec_merge_sort: Vector is destroyed");
  if (vec->container.size <= 1) return;

  array_merge_sort_rec(vec->container.buff, 0, vec->container.size - 1, vec->container.type_size, cmp);
}

void LinkedList_quick_sort(LinkedList *linked, int (*cmp)(const uint8_t *, const uint8_t *))
{
  Sort_basic_validations(linked, cmp);
  if (linked->state == DS_STATE_DESTROYED)
    RAISE(ExceptDestroyedDataStructure, "LinkedList_quick_sort: List is destroyed");
  if (linked->size <= 1) return;
  
  ll_quick_sort_rec(linked->tail, linked->head, cmp, linked->type_size);
}



void LinkedList_merge_sort(LinkedList *linked, int (*cmp)(const uint8_t *, const uint8_t *))
{
  Sort_basic_validations(linked, cmp);
  if (linked->state == DS_STATE_DESTROYED)
    RAISE(ExceptDestroyedDataStructure, "LinkedList_merge_sort: List is destroyed");
  if (linked->size <= 1) return;

  /* Run the sort starting from tail (first logical element) */
  Node *sorted = ll_merge_sort_rec(linked->tail, cmp);

  /* Reattach sorted chain to the list */
  linked->tail = sorted;

  /* Walk to the new last node to restore linked->head */
  Node *cursor = sorted;
  while (cursor->next != NULL)
    cursor = cursor->next;
  linked->head = cursor;
}
