#ifndef DS_MEM_INCLUDED
#define DS_MEM_INCLUDED

extern void *(*ds_mem_alloc)(unsigned long bytes);
extern void *(*ds_mem_ralloc)(void *addr, unsigned long bytes);
extern void (*ds_mem_free)(void *ptr);
extern void *(*ds_mem_copy)(void * restrict dst, const void * restrict src, long unsigned bytes);

#endif /* DS_MEM_INCLUDED */




