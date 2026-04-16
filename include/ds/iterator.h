#ifndef ITERATOR_INCLUDED
#define ITERATOR_INCLUDED

#include <inttypes.h>

// TODO: Add some exception for not initialized iterators
#define ITERATOR(type) \
  struct {					\
    Iterator iterator_st;			\
    type *foo_ptr;				\
  }


#define ITERATOR_GET_DATA(iterator)					\
  (*((typeof((iterator).foo_ptr)) (iterator).iterator_st.get_data((Iterator *) &(iterator))))


#define ITERATOR_INC(iterator)			\
  (iterator).iterator_st.inc((Iterator *) &(iterator))


#define ITERATOR_DEC(iterator)			\
  (iterator).iterator_st.dec((Iterator *) &(iterator))


#define ITERATOR_MOVE_AT(iterator, index)			\
  (iterator).iterator_st.move_at((Iterator *) &(iterator), index)


#define ITERATOR_CMP(iterator_a, iterator_b)			\
  (iterator).iterator_st.cmp((Iterator *) &(iterator_a), (Iterator *) &(iterator_b))


typedef struct Iterator_st {
  const uint8_t *ds;
  uint8_t *index_ptr;
  uint32_t index;
  
  void (*inc)(struct Iterator_st *iterator);
  void (*dec)(struct Iterator_st *iterator);
  const uint8_t *(*get_data)(struct Iterator_st *iterator);
  void (*move_at)(struct Iterator_st *iterator, uint32_t index);
  void (*cmp)(struct Iterator_st *iterator_a, struct Iterator_st *iterator_b);
} Iterator;

#endif /* ITERATOR_INCLUDED */
