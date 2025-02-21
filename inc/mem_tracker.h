// #ifndef MEMORY_TRACKER_H
// #define MEMORY_TRACKER_H

// #include <stdio.h>
// #include <stdlib.h>

// struct mem_block {
//   void *ptr;
//   size_t size;
//   struct mem_block *next;
// };

// void *nMalloc(size_t size, const char *file, const char *func, int line);
// void nFree(void *ptr, const char *file, const char *func, int line);

// // Ensure these macros are defined before any other includes
// #define malloc(size) nMalloc(size, __FILE__, __func__, __LINE__)
// #define free(ptr) nFree(ptr, __FILE__, __func__, __LINE__)

// #endif
