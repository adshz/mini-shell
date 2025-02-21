// #include "mem_tracker.h"
// #include <unistd.h>
// #include <fcntl.h>
// #include "libft.h"
// // Undefine the macros to use the original malloc and free
// #undef malloc
// #undef free

// static void write_str(int fd, const char *str) {
//     if (str)
//         write(fd, str, ft_strlen(str));
// }

// static void write_ptr(int fd, void *ptr) {
//     char buff[32];
//     snprintf(buff, sizeof(buff), "%p", ptr);
//     write_str(fd, buff);
// }

// static void write_size(int fd, size_t n) {
//     char buff[32];
//     snprintf(buff, sizeof(buff), "%zu", n);
//     write_str(fd, buff);
// }

// static void write_int(int fd, int n) {
//     char buff[32];
//     snprintf(buff, sizeof(buff), "%d", n);
//     write_str(fd, buff);
// }

// void *nMalloc(size_t size, const char *file, const char *func, int line) {
//     void *ptr = (void *)malloc(size);
//     char buff[128] = {0};
//     int fd;

//     snprintf(buff, 128, "./src/utils/mem_block/%p.mem", ptr);
//     fd = open(buff, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd < 0) {
//         free(ptr);
//         return NULL;
//     }

//     write_str(fd, "\033[0;33m[MALLOC] size: ");
//     write_size(fd, size);
//     write_str(fd, ", ptr: ");
//     write_ptr(fd, ptr);
//     write_str(fd, ", file: ");
//     write_str(fd, file);
//     write_str(fd, ", function: ");
//     write_str(fd, func);
//     write_str(fd, ", line: ");
//     write_int(fd, line);
//     write_str(fd, "\033[0m\n");

//     close(fd);
//     return ptr;
// }

// void nFree(void *ptr, const char *file, const char *func, int line) {
//     char buff[128] = {0};

//     if (!ptr)
//         return free(ptr);

//     snprintf(buff, 128, "./src/utils/mem_block/%p.mem", ptr);

//     if (unlink(buff) < 0) {
//         write_str(STDERR_FILENO, "\033[0;31m[FREE] ptr: ");
//         write_ptr(STDERR_FILENO, ptr);
//         write_str(STDERR_FILENO, ", file: ");
//         write_str(STDERR_FILENO, file);
//         write_str(STDERR_FILENO, ", function: ");
//         write_str(STDERR_FILENO, func);
//         write_str(STDERR_FILENO, ", line: ");
//         write_int(STDERR_FILENO, line);
//         write_str(STDERR_FILENO, "\033[0m\n");
//         return;
//     }
  
//     free(ptr);
// }

// #define malloc(size) nMalloc(size, __FILE__, __func__, __LINE__)
// #define free(ptr) nFree(ptr)
