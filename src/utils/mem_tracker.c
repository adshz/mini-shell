#include "mem_tracker.h"
#include <unistd.h>
// Undefine the macros to use the original malloc and free
#undef malloc
#undef free

void *nMalloc(size_t size, const char *file, const char *func, int line) {
    void *ptr = (void *)malloc(size);

    char buff[128] =  {0};

    snprintf(buff, 128, "./mem_block/%p", ptr);
    FILE *fp = fopen(buff, "w");
    if (!fp) {
      free(ptr);
      return NULL;
    }

    fprintf(fp, "\033[0;33m[MALLOC] size: %zu, ptr: %p, file: %s, function: %s, line: %d\033[0m\n", 
           size, ptr, file, func, line);
    fflush(fp);
    fclose(fp);
    return ptr;
}

void nFree(void *ptr, const char *file, const char *func, int line) {


   
    char buff[128] =  {0};

    snprintf(buff, 128, "./src/utils/mem_block/%p", ptr);

    if (unlink(buff) < 0) {
       printf("\033[0;31m[FREE] ptr: %p, file: %s, function: %s, line: %d\033[0m\n", 
           ptr, file, func, line);
      return ;
      //printf("double free detected %p\n", ptr);
    }
  
    return free(ptr);
}

#define malloc(size) nMalloc(size, __FILE__, __func__, __LINE__)
#define free(ptr) nFree(ptr)
