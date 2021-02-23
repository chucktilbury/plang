#ifndef __MEMORY_H__
#define __MEMORY_H__

#define CALLOC memory_calloc
#define MALLOC memory_malloc
#define REALLOC memory_realloc
#define STRDUP memory_strdup
#define ALLOC_DS(t) (t*)memory_calloc(1, sizeof(t))
#define FREE(p) memory_free((void*)p)

#include <stdlib.h>

void init_memory();
void *memory_calloc(size_t, size_t);
void *memory_malloc(size_t);
void *memory_realloc(void*, size_t);
void memory_free(void*);
char* memory_strdup(const char*);

#endif