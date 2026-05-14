#ifndef SORT_INCLUDED
#define SORT_INCLUDED

#include "iterator.h"
#include "vec.h"
#include "linked.h"

/* ══════════════════════════════════════════════════════════════════════════
 * Raw function declarations
 * ══════════════════════════════════════════════════════════════════════════ */

extern void Vec_merge_sort(Vec *vec, int (*cmp)(const uint8_t *, const uint8_t *));
extern void Vec_quick_sort(Vec *vec, int (*cmp)(const uint8_t *, const uint8_t *));

extern void LinkedList_merge_sort(LinkedList *linked, int (*cmp)(const uint8_t *, const uint8_t *));
extern void LinkedList_quick_sort(LinkedList *linked, int (*cmp)(const uint8_t *, const uint8_t *));

/* ══════════════════════════════════════════════════════════════════════════
 * Generic Dispatcher
 * ══════════════════════════════════════════════════════════════════════════ */

#define _SORT_DISPATCH(container, cmp, VEC_FUNC, LINKED_FUNC)           \
  _Generic((container)._foo_ds_type_ptr,				\
	   Vec * : VEC_FUNC((Vec *) &(container), (int (*)(const uint8_t *, const uint8_t *)) (cmp)), \
	   const Vec * : VEC_FUNC((Vec *) &(container), (int (*)(const uint8_t *, const uint8_t *)) (cmp)), \
	   LinkedList * : LINKED_FUNC((LinkedList *) &(container), (int (*)(const uint8_t *, const uint8_t *)) (cmp)), \
	   const LinkedList * : LINKED_FUNC((LinkedList *) &(container), (int (*)(const uint8_t *, const uint8_t *)) (cmp)) \
  )

/* ══════════════════════════════════════════════════════════════════════════
 * Sort Implementation Macros
 * ══════════════════════════════════════════════════════════════════════════ */

// Versión con comparador explícito
#define _SORT_2(container, cmp, V_FN, L_FN)	\
  _SORT_DISPATCH(container, cmp, V_FN, L_FN)

// Versión con comparador automático (usa el default del DS)
#define _SORT_1(container, V_FN, L_FN)					\
  _SORT_DISPATCH(container, ITERATOR_GENERIC_CMP_FUNC(container), V_FN, L_FN)

/* ══════════════════════════════════════════════════════════════════════════
 * Public API
 * ══════════════════════════════════════════════════════════════════════════ */

#define _SORT_PICK(_1, _2, NAME, ...) NAME

#define MERGE_SORT(container, ...)                                      \
  _SORT_PICK(container, ##__VA_ARGS__, _SORT_2, _SORT_1)		\
    (container, ##__VA_ARGS__, Vec_merge_sort, LinkedList_merge_sort)

#define QUICK_SORT(container, ...)                                      \
  _SORT_PICK(container, ##__VA_ARGS__, _SORT_2, _SORT_1)		\
    (container, ##__VA_ARGS__, Vec_quick_sort, LinkedList_quick_sort)

#endif /* SORT_INCLUDED */
