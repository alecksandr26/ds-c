#ifndef ITERATOR_INCLUDED
#define ITERATOR_INCLUDED

#include <inttypes.h>
#include "ds_exception.h"

#define ITERATOR(type)							\
  struct {								\
    const uint8_t *ds;							\
    const uint8_t *index_ptr;						\
    const uint32_t index;						\
    void (* const inc)(Iterator *iterator);				\
    void (* const dec)(Iterator *iterator);				\
    const uint8_t *(* const get_data)(Iterator *iterator);		\
    void (* const move_at)(Iterator *iterator, uint32_t index);		\
    int (* const cmp)(Iterator *iterator_a, Iterator *iterator_b);	\
    type *foo_ptr;							\
  }


#define ITERATOR_GET_DATA(iterator)                                     \
  __extension__ ({							\
      const uint8_t *__ds_data = (iterator).get_data((Iterator *) &(iterator)); \
      if (__ds_data == NULL)						\
	RAISE(ExceptInvalidArgument,					\
	  "Can't get data from a null iterator");			\
      *((typeof((iterator).foo_ptr)) __ds_data);			\
    })

#define ITERATOR_INC(iterator)				\
  (iterator).inc((Iterator *) &(iterator))


#define ITERATOR_DEC(iterator)				\
  (iterator).dec((Iterator *) &(iterator))


#define ITERATOR_MOVE_AT(iterator, index)				\
  (iterator).move_at((Iterator *) &(iterator), index)

#define ITERATOR_IS_NULL(iterator)				\
  ((iterator).get_data((Iterator *) &(iterator)) == NULL)


#define ITERATOR_CMP(iterator_a, iterator_b)                            \
  (((iterator_a).cmp == NULL)						\
    ? __extension__ ({							\
	RAISE(ExceptNullCmpFunction,					\
	  "No compare function assigned to this iterator — "		\
	  "pass a cmp to LINKED_LIST_BEGIN/END or use a type "		\
	  "supported by ITERATOR_GENERIC_CMP_FUNC");			\
	0;								\
	  })								\
    : (iterator_a).cmp(							\
      (Iterator *) &(iterator_a),					\
      (Iterator *) &(iterator_b)))


#define ITERATOR_GENERIC_CMP_FUNC(iterator)         \
  _Generic((iterator).foo_ptr,			    \
	   int           *: Iterator_cmp_int,	    \
	   long          *: Iterator_cmp_long,	    \
	   short         *: Iterator_cmp_short,	    \
	   unsigned int  *: Iterator_cmp_uint,	    \
	   unsigned long *: Iterator_cmp_ulong,	    \
	   unsigned short*: Iterator_cmp_ushort,    \
	   char          *: Iterator_cmp_str,	    \
	   default        : Iterator_cmp_raise_except	\
  )


typedef struct Iterator Iterator;

/* Signed integers */
extern int Iterator_cmp_int   (Iterator *a, Iterator *b);
extern int Iterator_cmp_long  (Iterator *a, Iterator *b);
extern int Iterator_cmp_short (Iterator *a, Iterator *b);

/* Unsigned integers */
extern int Iterator_cmp_uint   (Iterator *a, Iterator *b);
extern int Iterator_cmp_ulong  (Iterator *a, Iterator *b);
extern int Iterator_cmp_ushort (Iterator *a, Iterator *b);

/* Strings — lexicographic */
extern int Iterator_cmp_str (Iterator *a, Iterator *b);

/* No-op fallback */
extern int Iterator_cmp_raise_except(Iterator *a, Iterator *b);


extern void Iterator_set_cmp(Iterator *iterator, int (* const cmp)(Iterator * , Iterator *));

#ifdef ITERATOR_INTERNAL
struct Iterator {
  const uint8_t *ds;
  uint8_t *index_ptr;
  uint32_t index;
  
  void (*inc)(Iterator *iterator);
  void (*dec)(Iterator *iterator);
  const uint8_t *(*get_data)(Iterator *iterator);
  void (*move_at)(Iterator *iterator, uint32_t index);
  int (*cmp)(Iterator *iterator_a, Iterator *iterator_b);
};

extern void Iterator_init(
  Iterator *iterator,
  const uint8_t   *ds,
  uint8_t   *index_ptr,
  uint32_t         index,
  void           (* const inc)     (Iterator *),
  void           (* const dec)     (Iterator *),
  const uint8_t *(* const get_data)(Iterator *),
  void           (* const move_at) (Iterator *, uint32_t),
  int            (* const cmp)     (Iterator *, Iterator *));

extern void Iterator_basic_validations(Iterator *iterator);

#endif

#endif /* ITERATOR_INCLUDED */
