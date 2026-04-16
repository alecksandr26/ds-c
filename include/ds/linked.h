#ifndef LINKED_INCLUDED
#define LINKED_INCLUDED

#include <inttypes.h>

#include "iterator.h"

#define LINKED_LIST(type)			\
  struct {					\
    const uint8_t *head, *tail;			\
    const uint32_t type_size, size;		\
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

#define LINKED_LIST_PUSH_BACK(linked, new_val)	\
  __extension__ ({				\
      typeof(*(linked).foo_ptr) __ds_linked_new_element = (new_val);	\
      LinkedList_push_back((LinkedList *) &(linked), (const uint8_t *) &__ds_linked_new_element); \
    })


#define LINKED_LIST_PUSH_FRONT(linked, new_val)	\
  __extension__ ({				\
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


#define LINKED_LIST_CLEAR(linked) \
  LinkedList_clear((LinkedList *) &(linked))

#define LINKED_LIST_DESTROY(linked) \
  LinkedList_destroy((LinkedList *) &(linked))


#define LINKED_LIST_BEGIN(linked, iterator)	\
  __extension__ ({				\
      LinkedList_begin((LinkedList *) &(linked), (Iterator *) &(iterator)); \
      (iterator);							\
    })


#define LINKED_LIST_END(linked, iterator)	\
  __extension__ ({				\
      LinkedList_end((LinkedList *) &(linked), (Iterator *) &(iterator)); \
      (iterator);							\
    })


typedef struct Node_st {
  struct Node_st *next, *prev;
  const uint8_t *data_ptr;
  // the data will go here ...
} Node;

typedef struct {
  Node *head, *tail;
  uint32_t type_size, size;
} LinkedList;



extern void LinkedList_init(LinkedList *linked, uint32_t type_size);
extern const uint8_t *LinkedList_front(LinkedList *linked);
extern const uint8_t *LinkedList_back(LinkedList *linked);
extern void LinkedList_push_back(LinkedList *linked, const uint8_t *data);
extern void LinkedList_push_front(LinkedList *linked, const uint8_t *data);
extern void LinkedList_pop_back(LinkedList *linked);
extern void LinkedList_pop_front(LinkedList *linked);
extern void LinkedList_clear(LinkedList *linked);
extern void LinkedList_destroy(LinkedList *linked);
extern void LinkedList_begin(LinkedList *linked, Iterator *iterator);
extern void LinkedList_end(LinkedList *linked, Iterator *iterator);


#endif /* LINKED_INCLUDED */




