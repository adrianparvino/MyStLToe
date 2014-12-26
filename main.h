#include <stddef.h>

struct string {
    char *c_string;
    size_t u_length;
    size_t i_size;    
};

struct string *mystltoe_make_string(const char c_string[], const size_t size);
void	       mystltoe_destroy_string(struct string *str);
void	       mystltoe_insert(struct string *dest, struct string *src, size_t u);
struct string *mystltoe_split(struct string *src, size_t u);
void	       mystltoe_append(struct string *dest, struct string *src);
void	       mystltoe_append_c_string(struct string *dest, const char *src, const size_t size);
