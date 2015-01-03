#include <stddef.h>
#include <stdint.h>

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

uint8_t i_from_u_char(const char c_string);
size_t u_length_from_c_string(const char *c_string, const size_t size);
size_t i_from_u_index(const char *c_string, size_t u_index);
struct string *make_string_raw(char *c_string, const size_t i_size, const size_t u_length);
char *split_raw(struct string *src, const size_t i);
void append_raw(struct string *dest, const char *src, const size_t i_size);
