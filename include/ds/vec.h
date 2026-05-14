#ifndef VEC_INCLUDED
#define VEC_INCLUDED

#include <inttypes.h>
#include "ds_container.h"
#include "consts.h"
#include "iterator.h"

#define DEFINE_VEC_TYPE(type)			\
  struct ds_vec_ ## type ## _t {			\
    const type *buff;				\
    const uint32_t capacity, size, type_size;	\
    const DS_State state;			\
    const type *_foo_type_ptr;			\
    const Vec *_foo_ds_type_ptr;		\
  };						\
  typedef struct ds_vec_ ## type ## _t ds_vec_ ## type ## _t

#define VEC(type, ...)				\
  ds_vec_ ## type ## _t

#define VEC_INIT(vec, ...)					\
  __extension__ ({						\
      Vec_init((Vec *) &(vec),					\
	__VA_OPT__(1 ? __VA_ARGS__ :) DEFAULT_VEC_CAPACITY,	\
	sizeof(typeof(*(vec).buff)));				\
      (vec);							\
    })

#define VEC_CAPACITY(vec) ((vec).capacity)
#define VEC_SIZE(vec) ((vec).size)

#define VEC_PUSH_BACK(vec, new_val)					\
  __extension__ ({							\
      typeof(*(vec).buff) __ds_vec_new_element = (new_val);		\
      Vec_push_back((Vec *) &(vec), (const uint8_t *) &__ds_vec_new_element);	\
    })

#define VEC_PUSH_FRONT(vec, new_val)					\
  __extension__ ({							\
      typeof(*(vec).buff) __ds_vec_new_element = (new_val);		\
      Vec_push_front((Vec *) &(vec), (const uint8_t *) &__ds_vec_new_element);	\
    })

#define VEC_POP_BACK(vec)					\
  (*((typeof((vec).buff[0]) *) Vec_pop_back((Vec *) &(vec))))

#define VEC_POP_FRONT(vec)					\
  (*((typeof((vec).buff[0]) *) Vec_pop_front((Vec *) &(vec))))

#define VEC_GET(vec, index)					\
  (*((typeof((vec).buff[0]) *) Vec_get((Vec *) &(vec), index)))

#define VEC_INSERT(vec, index, new_val)					\
  __extension__ ({							\
      typeof(*(vec).buff) __ds_vec_insert_element = (new_val);		\
      Vec_insert((Vec *) &(vec), index, (const uint8_t *) &__ds_vec_insert_element); \
    })

#define VEC_REMOVE(vec, index)			\
  Vec_remove((Vec *) &(vec), index)

#define VEC_REPLACE(vec, index, new_val)				\
  __extension__ ({							\
      typeof(*(vec).buff) __ds_vec_insert_element = (new_val);		\
      Vec_replace((Vec *) &(vec), index, (const uint8_t *) &__ds_vec_insert_element); \
    })

#define VEC_CLEAR(vec)				\
  Vec_clear((Vec *) &(vec))

#define VEC_DESTROY(vec)			\
  Vec_destroy((Vec *) &(vec))

#define _VEC_BEGIN_2(vec, iterator, cmp)	\
  __extension__ ({				\
      Vec_begin(				\
	(Vec *) &(vec),				\
	(Iterator *) &(iterator),		\
	(cmp)					\
      );					\
      (iterator);				\
    })

#define _VEC_BEGIN_1(vec, iterator)		\
  __extension__ ({				\
      Vec_begin(				\
	(Vec *) &(vec),				\
	(Iterator *) &(iterator),		\
	ITERATOR_GENERIC_CMP_FUNC(iterator)	\
      );					\
      (iterator);				\
    })

#define _VEC_END_2(vec, iterator, cmp)		\
  __extension__ ({				\
      Vec_end(					\
	(Vec *) &(vec),				\
	(Iterator *) &(iterator),		\
	(cmp)					\
      );					\
      (iterator);				\
    })

#define _VEC_END_1(vec, iterator)		\
  __extension__ ({				\
      Vec_end(					\
	(Vec *) &(vec),				\
	(Iterator *) &(iterator),		\
	ITERATOR_GENERIC_CMP_FUNC(iterator)	\
      );					\
      (iterator);				\
    })

#define _VEC_PICK(_1, _2, _3, NAME, ...) NAME

#define VEC_BEGIN(vec, iterator, ...)		\
  _VEC_PICK(					\
    vec, iterator, ##__VA_ARGS__,		\
    _VEC_BEGIN_2,				\
    _VEC_BEGIN_1				\
  )(vec, iterator, ##__VA_ARGS__)

#define VEC_END(vec, iterator, ...)		\
  _VEC_PICK(					\
    vec, iterator, ##__VA_ARGS__,		\
    _VEC_END_2,					\
    _VEC_END_1					\
  )(vec, iterator, ##__VA_ARGS__)


#define _VEC_GET_ITERATOR_AT_2(vec, iterator, index, cmp)  \
    __extension__ ({                                        \
        Vec_begin(                                          \
            (Vec *) &(vec),                                 \
            (Iterator *) &(iterator),                       \
            (cmp)                                           \
        );                                                  \
        ITERATOR_MOVE_AT(iterator, index);                  \
        (iterator);                                         \
    })

#define _VEC_GET_ITERATOR_AT_1(vec, iterator, index)        \
    __extension__ ({                                        \
        Vec_begin(                                          \
            (Vec *) &(vec),                                 \
            (Iterator *) &(iterator),                       \
            ITERATOR_GENERIC_CMP_FUNC(iterator)             \
        );                                                  \
        ITERATOR_MOVE_AT(iterator, index);                  \
        (iterator);                                         \
    })

#define _VEC_GET_ITERATOR_AT_PICK(_1, _2, _3, _4, NAME, ...) NAME

#define VEC_GET_ITERATOR_AT(vec, iterator, index, ...)      \
    _VEC_GET_ITERATOR_AT_PICK(                              \
        vec, iterator, index, ##__VA_ARGS__,                \
        _VEC_GET_ITERATOR_AT_2,                             \
        _VEC_GET_ITERATOR_AT_1                              \
    )(vec, iterator, index, ##__VA_ARGS__)

typedef struct Vec Vec;

extern void Vec_init(Vec *vec, uint32_t capacity, uint32_t type_size);
extern void Vec_push_back(Vec *vec, const uint8_t *new_val);
extern void Vec_push_front(Vec *vec, const uint8_t *new_val);
extern uint8_t *Vec_pop_back(Vec *vec);
extern uint8_t *Vec_pop_front(Vec *vec);
extern uint8_t *Vec_get(Vec *vec, uint32_t index);
extern void Vec_insert(Vec *vec, uint32_t index, const uint8_t *new_val);
extern void Vec_remove(Vec *vec, uint32_t index);
extern void Vec_replace(Vec *vec, uint32_t index, const uint8_t *new_value);
extern void Vec_clear(Vec *vec);
extern void Vec_destroy(Vec *vec);
extern void Vec_begin(Vec *vec, Iterator *iterator, int (*const cmp)(const uint8_t *, const uint8_t *));
extern void Vec_end(Vec *vec, Iterator *iterator, int (*const cmp)(const uint8_t *, const uint8_t *));



#ifdef VEC_PRIVATE

struct Vec {
  Container container;
  DS_State state;
};

#endif

DEFINE_VEC_TYPE(int);
DEFINE_VEC_TYPE(long);
DEFINE_VEC_TYPE(short);
DEFINE_VEC_TYPE(int8_t);
DEFINE_VEC_TYPE(int16_t);
DEFINE_VEC_TYPE(int32_t);
DEFINE_VEC_TYPE(int64_t);

DEFINE_VEC_TYPE(uint8_t);
DEFINE_VEC_TYPE(uint16_t);
DEFINE_VEC_TYPE(uint32_t);
DEFINE_VEC_TYPE(uint64_t);
DEFINE_VEC_TYPE(size_t);


DEFINE_VEC_TYPE(float);
DEFINE_VEC_TYPE(double);
DEFINE_VEC_TYPE(bool);


#endif
