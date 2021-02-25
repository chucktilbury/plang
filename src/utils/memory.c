/*
    This is a simple wrapper around the memory allocation routines to make
    error handling easier. All memory allocation errors are fatal errors.
*/
#include "common.h"

static uint64_t mem_segment;
#define SEG_MASK 0xFFFF00000000
#define GET_SEG(p) (((uint64_t)p)&SEG_MASK)

void init_memory() {

    // int stackvar;
    void* ptr = malloc(1);
    // fprintf(stderr, "stk=%p, mal=%p, glo=%p\n", &stackvar, ptr, &mem_segment);
    mem_segment = GET_SEG(ptr);
}

void *memory_calloc(const char* file, const char* func, int line, size_t num, size_t size) {

    void* ptr = calloc(num, size);
    LOC_ASSERT(file, func, line, ptr != NULL, "cannot calloc %lu bytes\n", num*size);
    return ptr;
}

void *memory_malloc(const char* file, const char* func, int line, size_t size) {

    void* ptr = malloc(size);
    LOC_ASSERT(file, func, line, ptr != NULL, "cannot malloc %lu bytes\n", size);

    return ptr;
}

void *memory_realloc(const char* file, const char* func, int line, void* ptr, size_t size) {

    void* nptr = realloc(ptr, size);
    LOC_ASSERT(file, func, line, nptr != NULL, "cannot reallocate %lu bytes\n", size);

    return nptr;
}

void memory_free(const char* file, const char* func, int line, void* ptr) {

    if(mem_segment != GET_SEG(ptr))
        fatal_error("%s: %s:%d assert failed: Attempt to free a pointer that was not allocated: %p\n", file, func, line, ptr);
    else
        free(ptr);
}

char* memory_strdup(const char* file, const char* func, int line, const char* str) {

    char* nptr = strdup(str);
    LOC_ASSERT(file, func, line, nptr != NULL, "cannot strdup %lu bytes\n", strlen(str));

    return nptr;
}
