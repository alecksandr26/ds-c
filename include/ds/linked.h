#ifndef LINKED_INCLUDED
#define LINKED_INCLUDED

#include <inttypes.h>

#include "iterator.h"
#include "consts.h"

#define LINKED_LIST(type)			\
  struct {					\
    const uint8_t *head, *tail;			\
    const uint32_t type_size, size;		\
    const DS_State state;			\
    const type * const foo_ptr;			\
  }


#define LINKED_LIST_INIT(linked)					\
  __extension__ ({							\
      LinkedList_init((LinkedList *) &(linked), sizeof(typeof(*(linked).foo_ptr))); \
      (linked);								\
    })


#define LINKED_LIST_SIZE(linked) ((linked).size)


#define LINKED_LIST_FRONT(linked)					\
  (*((typeof((linked).foo_ptr)) LinkedList_front((LinkedList *) &(linked))))

#define LINKED_LIST_BACK(linked)					\
  (*((typeof((linked).foo_ptr)) LinkedList_back((LinkedList *) &(linked))))

#define LINKED_LIST_PUSH_BACK(linked, new_val)				\
  __extension__ ({							\
      typeof(*(linked).foo_ptr) __ds_linked_new_element = (new_val);	\
      LinkedList_push_back((LinkedList *) &(linked), (const uint8_t *) &__ds_linked_new_element); \
    })


#define LINKED_LIST_PUSH_FRONT(linked, new_val)				\
  __extension__ ({							\
      typeof(*(linked).foo_ptr) __ds_linked_new_element = (new_val);	\
      LinkedList_push_front((LinkedList *) &(linked), (const uint8_t *) &__ds_linked_new_element); \
    })

#define LINKED_LIST_POP_BACK(linked)					\
  __extension__ ({							\
      typeof(*(linked).foo_ptr) __ds_linked_remove_element = LINKED_LIST_BACK(linked); \
      LinkedList_pop_back((LinkedList *) &(linked));			\
      (__ds_linked_remove_element);					\
    })
  


#define LINKED_LIST_POP_FRONT(linked)					\
  __extension__ ({							\
      typeof(*(linked).foo_ptr) __ds_linked_remove_element = LINKED_LIST_FRONT(linked); \
      LinkedList_pop_front((LinkedList *) &(linked));			\
      (__ds_linked_remove_element);					\
    })


#define LINKED_LIST_CLEAR(linked)		\
  LinkedList_clear((LinkedList *) &(linked))

#define LINKED_LIST_DESTROY(linked)		\
  LinkedList_destroy((LinkedList *) &(linked))


#define _LINKED_LIST_BEGIN_2(linked, iterator, cmp)	\
  __extension__ ({					\
      LinkedList_begin(					\
	(LinkedList *) &(linked),			\
	(Iterator *)   &(iterator),			\
	(cmp)						\
      );						\
      (iterator);					\
    })

#define _LINKED_LIST_BEGIN_1(linked, iterator)	\
  __extension__ ({				\
      LinkedList_begin(				\
	(LinkedList *) &(linked),		\
	(Iterator *)   &(iterator),		\
	ITERATOR_GENERIC_CMP_FUNC(iterator)	\
      );					\
      (iterator);				\
    })

#define _LINKED_LIST_END_2(linked, iterator, cmp)	\
  __extension__ ({					\
      LinkedList_end(					\
	(LinkedList *) &(linked),			\
	(Iterator *)   &(iterator),			\
	(cmp)						\
      );						\
      (iterator);					\
    })

#define _LINKED_LIST_END_1(linked, iterator)	\
  __extension__ ({				\
      LinkedList_end(				\
	(LinkedList *) &(linked),		\
	(Iterator *)   &(iterator),		\
	ITERATOR_GENERIC_CMP_FUNC(iterator)	\
      );					\
      (iterator);				\
    })

#define _LINKED_LIST_PICK(_1, _2, _3, NAME, ...) NAME

#define LINKED_LIST_BEGIN(linked, iterator, ...)	\
  _LINKED_LIST_PICK(					\
    linked, iterator, ##__VA_ARGS__,			\
    _LINKED_LIST_BEGIN_2,				\
    _LINKED_LIST_BEGIN_1				\
  )(linked, iterator, ##__VA_ARGS__)

#define LINKED_LIST_END(linked, iterator, ...)	\
  _LINKED_LIST_PICK(				\
    linked, iterator, ##__VA_ARGS__,		\
    _LINKED_LIST_END_2,				\
    _LINKED_LIST_END_1				\
  )(linked, iterator, ##__VA_ARGS__)


#define LINKED_LIST_BEGIN(linked, iterator, ...)	\
  _LINKED_LIST_PICK(					\
    linked, iterator, ##__VA_ARGS__,			\
    _LINKED_LIST_BEGIN_2,				\
    _LINKED_LIST_BEGIN_1				\
  )(linked, iterator, ##__VA_ARGS__)

#define LINKED_LIST_END(linked, iterator, ...)	\
  _LINKED_LIST_PICK(				\
    linked, iterator, ##__VA_ARGS__,		\
    _LINKED_LIST_END_2,				\
    _LINKED_LIST_END_1				\
  )(linked, iterator, ##__VA_ARGS__)


#define _LINKED_LIST_GET_ITERATOR_AT_2(linked, iterator, index, cmp)    \
  __extension__ ({							\
      LinkedList_begin(							\
	(LinkedList *) &(linked),					\
	(Iterator *) &(iterator),					\
	(cmp)								\
      );								\
      ITERATOR_MOVE_AT(iterator, index);				\
      (iterator);							\
    })

#define _LINKED_LIST_GET_ITERATOR_AT_1(linked, iterator, index)	\
  __extension__ ({						\
      LinkedList_begin(						\
	(LinkedList *) &(linked),				\
	(Iterator *) &(iterator),				\
	ITERATOR_GENERIC_CMP_FUNC(iterator)			\
      );							\
      ITERATOR_MOVE_AT(iterator, index);			\
      (iterator);						\
    })

#define _LINKED_LIST_GET_ITERATOR_AT_PICK(_1, _2, _3, _4, NAME, ...) NAME

#define LINKED_LIST_GET_ITERATOR_AT(linked, iterator, index, ...)       \
  _LINKED_LIST_GET_ITERATOR_AT_PICK(					\
    linked, iterator, index, ##__VA_ARGS__,				\
    _LINKED_LIST_GET_ITERATOR_AT_2,					\
    _LINKED_LIST_GET_ITERATOR_AT_1					\
  )(linked, iterator, index, ##__VA_ARGS__)



typedef struct LinkedList LinkedList;

extern void LinkedList_init(LinkedList *linked, uint32_t type_size);
extern const uint8_t *LinkedList_front(LinkedList *linked);
extern const uint8_t *LinkedList_back(LinkedList *linked);
extern void LinkedList_push_back(LinkedList *linked, const uint8_t *data);
extern void LinkedList_push_front(LinkedList *linked, const uint8_t *data);
extern void LinkedList_pop_back(LinkedList *linked);
extern void LinkedList_pop_front(LinkedList *linked);
extern void LinkedList_clear(LinkedList *linked);
extern void LinkedList_destroy(LinkedList *linked);
extern void LinkedList_begin(LinkedList *linked, Iterator *iterator, int (* const cmp)(Iterator *, Iterator *));
extern void LinkedList_end(LinkedList *linked, Iterator *iterator, int (* const cmp)(Iterator *, Iterator *));


#endif /* LINKED_INCLUDED */




