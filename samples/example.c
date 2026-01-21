#include <stdio.h>
#include <stddef.h>

#include <mem.h>

/* https://stackoverflow.com/questions/1238016/are-compound-statements-blocks-surrounded-by-parens-expressions-in-ansi-c */

#define DEFAULT_VEC_CAPACITY 100

#define VEC(type, ...)				\
  struct {					\
    type *buff;					\
    size_t capacity, size, type_size;		\
  }

#define VEC_INIT(vec, ...)						\
  __extension__ ({							\
      (vec).type_size = sizeof(typeof(*(vec).buff));			\
      (vec).size = 0;							\
      (vec).capacity = __VA_OPT__(1 ? __VA_ARGS__ :) DEFAULT_VEC_CAPACITY; \
      NEW((vec).buff, typeof(*(vec).buff)[(vec).capacity]);		\
      (vec);								\
    })

#define VEC_CAPACITY(vec) (vec).capacity
#define VEC_SIZE(vec) (vec).size
#define VEC_PUSH_BACK(vec, item) (vec).buff[(vec).size++] = item

#define VEC_POP_BACK(vec) (vec).buff[--(vec).size]

#define VEC_GET(vec, index) (vec).buff[index]

#define VEC_DESTROY(vec)

#include <string.h>

typedef struct {
  char *str;
  size_t length;
} String;

#define STRING_INIT(string, str_c)				\
  __extension__ ({						\
      (string).length = strlen(str_c);				\
      (string).str = NEW((string).str, char[(string).length]);	\
      memcpy((string).str, str_c, (string).length);		\
      (string);							\
    })


int main(void)
{
  VEC(int) *vec = NEW(vec);

  VEC_INIT(*vec, 100);

  int a = 10;
  VEC_PUSH_BACK(*vec, a);
  VEC_PUSH_BACK(*vec, 20);
  VEC_PUSH_BACK(*vec, 30);
	
  VEC_GET(*vec, 0) = 200;
  printf("%d\n", mem_dbg_is_freeded(vec));

  VEC(String) strings = VEC_INIT(strings, 100);
  STRING_INIT(VEC_GET(strings, 0), "Pedrito");
  printf("%s, %p\n", VEC_GET(strings, 0).str, VEC_GET(strings, 0).str);
	
  VEC(VEC(double)) mat = VEC_INIT(mat, 100);
  VEC_GET(mat, 0) = VEC_INIT(VEC_GET(mat, 0), 100);

  MemStats_T stats;
  mem_dbg_fetch_mem_stats(&stats, 3, 1);
	
  FREE(vec);
	
	
  return 0;
}

