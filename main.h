#include <stddef.h>

struct string {
    char *c_string;
    size_t u_length;
    size_t size;    
};

struct string *make_string(const char c_string[], const size_t size);
void destroy_string(struct string *str);
void insert(struct string *dest, struct string *src, size_t u);
struct string *split(struct string *src, size_t u);
void append(struct string *dest, struct string *src);
struct string *make_string_from_read(int fd, size_t n);
int write_from_string(int fd, struct string *src);
