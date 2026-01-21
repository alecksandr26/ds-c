#include <mem.h>
#include <string.h>
#include "../include/ds/ds_mem.h"

// Set default my own allocators
void *(*ds_mem_alloc)(size_t bytes) = mem_alloc;
void *(*ds_mem_ralloc)(void *addr, size_t bytes) = mem_ralloc;
void (*ds_mem_free)(void *ptr) = mem_free;

// TODO: Implement in the mem lib the memory copy func
void (*ds_mem_copy)(void *dst, void *src, size_t bytes) = memcpy;

