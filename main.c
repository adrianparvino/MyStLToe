#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>


#define BIT_TEST(x, y) (((x) & (y)) == (y))

struct string {
    char   *c_string;
    size_t u_length;
    size_t i_size;    
};

uint8_t i_from_u_char(const char c_string)
{
    return (BIT_TEST(c_string, 0xfc)) ? 6
	 : (BIT_TEST(c_string, 0xf8)) ? 5
	 : (BIT_TEST(c_string, 0xf0)) ? 4
	 : (BIT_TEST(c_string, 0xe0)) ? 3
	 : (BIT_TEST(c_string, 0xc0)) ? 2
	 :                              1;
}

size_t u_length_from_c_string(const char *c_string, const size_t size)
{
    size_t u_length = 0;
    size_t i = 0;
    while(i < size)
    {
	i += i_from_u_char(c_string[i]);
	++u_length;
    }
    return u_length;
}

size_t i_from_u_index(const char *c_string, size_t u_index)
{
    if (!u_index) return 0;
    int i = 0;
    do i += i_from_u_char(c_string[i]); while(--u_index);
    return i;
}

struct string *make_string_raw(char *c_string, const size_t i_size, const size_t u_length)
{
    struct string *str = malloc(sizeof(struct string));

    str->c_string = c_string;
    str->i_size	  = i_size;
    str->u_length = u_length;

    return str;
}

struct string *mystltoe_make_string(const char c_string[], const size_t size)
{
    char *c_string_copy = malloc(size);
    memcpy(c_string_copy, c_string, size);
	   
    return make_string_raw(c_string_copy, size, u_length_from_c_string(c_string_copy, size));
}

void mystltoe_destroy_string(struct string *str)
{
    free(str->c_string);
    free(str);
}

char *split_raw(struct string *src, const size_t i)
{
    char *dest = malloc(src->i_size - i);
    memcpy(dest, src->c_string + i, src->i_size - i);

    src->c_string = realloc(src->c_string, i);
    return dest;
}

struct string *mystltoe_split(struct string *src, const size_t u)
{
    int i = i_from_u_index(src->c_string, u);

    struct string *dest = make_string_raw(split_raw(src, i), src->i_size - i, src->u_length - u);

    src->i_size = i;
    src->u_length = u;

    return dest;
}

void append_raw(struct string *dest, const char *src, const size_t i_size)
{
    dest->c_string = realloc(dest->c_string, dest->i_size + i_size);
    memcpy(dest->c_string + dest->i_size, src, i_size);
}

void mystltoe_append(struct string *dest, const struct string *src)
{
    append_raw(dest, src->c_string, src->i_size);

    dest->i_size   += src->i_size;
    dest->u_length += src->u_length;
}

void mystltoe_insert(struct string *dest, const struct string *src, const size_t u)
{
    int i = i_from_u_index(dest->c_string, u);
    char *c_string = split_raw(dest, i);

    append_raw(dest, src->c_string, src->i_size);
    append_raw(dest, c_string, dest->i_size - i);

    dest->u_length += src->u_length;
    dest->i_size   += src->i_size;
}

void mystltoe_append_c_string(struct string *dest, const char *src, const size_t i_size)
{
    append_raw(dest, src, i_size);

    dest->i_size += i_size;
    dest->u_length += u_length_from_c_string(src, i_size);
}
