#ifndef DS_MEM_INCLUDED
#define DS_MEM_INCLUDED

#include <stddef.h>

extern void *(*ds_mem_alloc)(size_t bytes);
extern void *(*ds_mem_ralloc)(void *addr, size_t bytes);
extern void (*ds_mem_free)(void *ptr);

#endif /* DS_MEM_INCLUDED */




