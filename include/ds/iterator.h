#ifndef ITERATOR_INCLUDED
#define ITERATOR_INCLUDED

#include <inttypes.h>
#include "ds_exception.h"

#define DEFINE_ITERATOR_TYPE(type)					\
  struct ds_iterator_ ## type ## _t {					\
    const uint8_t *ds;							\
    const uint8_t *index_ptr;						\
    const uint32_t index;						\
    void (* const inc)(Iterator *iterator);				\
    void (* const dec)(Iterator *iterator);				\
    const uint8_t *(* const get_data)(const Iterator *iterator);	\
    void (* const move_at)(Iterator *iterator, uint32_t index);		\
    void (* const insert)(Iterator *iterator, const uint8_t *data);	\
    void (* const remove)(Iterator *iterator);				\
    void (* const replace)(Iterator *iterator, const uint8_t *data);	\
    int (* const cmp)(const uint8_t *a, const uint8_t *b); \
    const type *_foo_type_ptr;					\
  };									\
  typedef struct ds_iterator_ ## type ## _t ds_iterator_ ## type ## _t


#define ITERATOR(type)				\
  ds_iterator_ ## type ## _t 


#define ITERATOR_GET_DATA(iterator)                                     \
  __extension__ ({							\
      const uint8_t *__ds_data = (iterator).get_data((Iterator *) &(iterator)); \
      if (__ds_data == NULL)						\
	RAISE(ExceptInvalidArgument,					\
	  "Can't get data from a null iterator");			\
      *((typeof((iterator)._foo_type_ptr)) __ds_data);			\
    })

#define ITERATOR_INC(iterator)				\
  (iterator).inc((Iterator *) &(iterator))


#define ITERATOR_DEC(iterator)				\
  (iterator).dec((Iterator *) &(iterator))


#define ITERATOR_MOVE_AT(iterator, index)				\
  (iterator).move_at((Iterator *) &(iterator), index)

#define ITERATOR_IS_NULL(iterator)				\
  ((iterator).get_data((Iterator *) &(iterator)) == NULL)

#define ITERATOR_INSERT(iterator, new_value)				\
  __extension__ ({							\
      typeof(*(iterator)._foo_type_ptr) __ds_vec_insert_element = (new_value); \
      (iterator).insert((Iterator *) &(iterator), (const uint8_t *) &__ds_vec_insert_element); \
    })

#define ITERATOR_REMOVE(iterator)		\
  (iterator).remove((Iterator *) &(iterator))

#define ITERATOR_REPLACE(iterator, new_value)				\
  __extension__ ({							\
      typeof(*(iterator)._foo_type_ptr) __ds_vec_insert_element = (new_value); \
      (iterator).replace((Iterator *) &(iterator), (const uint8_t *) &__ds_vec_insert_element); \
    })


#define ITERATOR_CMP(iterator_a, iterator_b)                                   \
  (((iterator_a).cmp == NULL)						\
    ? __extension__ ({							\
        RAISE(ExceptNullCmpFunction,					\
          "No compare function assigned to this iterator — "		\
          "pass a cmp to LINKED_LIST_BEGIN/END or use a type "		\
          "supported by ITERATOR_GENERIC_CMP_FUNC");			\
        0;								\
      })								\
    : __extension__ ({							\
        const uint8_t *__cmp_da = (iterator_a).get_data((Iterator *) &(iterator_a)); \
        const uint8_t *__cmp_db = (iterator_b).get_data((Iterator *) &(iterator_b)); \
        (iterator_a).cmp(__cmp_da, __cmp_db);				\
      }))


#define ITERATOR_GENERIC_CMP_FUNC(iterator)         \
  _Generic((iterator)._foo_type_ptr,		    \
	   const int           *: Iterator_cmp_int,	    \
	   const long          *: Iterator_cmp_long,	    \
	   const short         *: Iterator_cmp_short,	    \
	   const unsigned int  *: Iterator_cmp_uint,	    \
	   const unsigned long *: Iterator_cmp_ulong,	    \
	   const unsigned short*: Iterator_cmp_ushort,    \
	   const char          *: Iterator_cmp_str,	    \
	   default        : Iterator_cmp_raise_except	\
  )


typedef struct Iterator Iterator;

/* Signed integers */
extern int Iterator_cmp_int   (const uint8_t *a, const uint8_t *b);
extern int Iterator_cmp_long  (const uint8_t *a, const uint8_t *b);
extern int Iterator_cmp_short (const uint8_t *a, const uint8_t *b);

/* Unsigned integers */
extern int Iterator_cmp_uint   (const uint8_t *a, const uint8_t *b);
extern int Iterator_cmp_ulong  (const uint8_t *a, const uint8_t *b);
extern int Iterator_cmp_ushort (const uint8_t *a, const uint8_t *b);

/* Strings — lexicographic */
extern int Iterator_cmp_str (const uint8_t *a, const uint8_t *b);

/* No-op fallback */
extern int Iterator_cmp_raise_except(const uint8_t *a, const uint8_t *b);


extern void Iterator_set_cmp(Iterator *iterator, int (* const cmp)(const uint8_t * , const uint8_t *));

#ifdef ITERATOR_PRIVATE
struct Iterator {
  const uint8_t *ds;
  uint8_t *index_ptr;
  uint32_t index;
  
  void (*inc)(Iterator *iterator);
  void (*dec)(Iterator *iterator);
  const uint8_t *(*get_data)(const Iterator *iterator);
  void (*move_at)(Iterator *iterator, uint32_t index);
  void (*insert)(Iterator *iterator, const uint8_t *data);
  void (*remove)(Iterator *iterator);
  void (*replace)(Iterator *iterator, const uint8_t *data);
  int (*cmp)(const uint8_t *a, const uint8_t *b);
};

extern void Iterator_init(
  Iterator *iterator,
  const uint8_t   *ds,
  uint8_t   *index_ptr,
  uint32_t         index,
  void           (* const inc)     (Iterator *),
  void           (* const dec)     (Iterator *),
  const uint8_t *(* const get_data)(const Iterator *),
  void           (* const move_at) (Iterator *, uint32_t),
  void           (* const insert)(Iterator *, const uint8_t *),
  void           (* const remove)(Iterator *),
  void           (* const replace)(Iterator *, const uint8_t *),
  int            (* const cmp)     (const uint8_t *, const uint8_t *));

extern void Iterator_basic_validations(const Iterator *iterator);

#endif

DEFINE_ITERATOR_TYPE(int);
DEFINE_ITERATOR_TYPE(long);
DEFINE_ITERATOR_TYPE(short);
DEFINE_ITERATOR_TYPE(int8_t);
DEFINE_ITERATOR_TYPE(int16_t);
DEFINE_ITERATOR_TYPE(int32_t);
DEFINE_ITERATOR_TYPE(int64_t);

DEFINE_ITERATOR_TYPE(uint8_t);
DEFINE_ITERATOR_TYPE(uint16_t);
DEFINE_ITERATOR_TYPE(uint32_t);
DEFINE_ITERATOR_TYPE(uint64_t);
DEFINE_ITERATOR_TYPE(size_t);

DEFINE_ITERATOR_TYPE(float);
DEFINE_ITERATOR_TYPE(double);
DEFINE_ITERATOR_TYPE(bool);

#endif /* ITERATOR_INCLUDED */
