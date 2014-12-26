#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

struct string {
    char   *c_string;
    size_t length;
    size_t size;    
};

#define bit_Test(x, y) (((x) & (y)) == (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

size_t make_length(const char c_string[], const size_t size)
{
    size_t length = 0;
    size_t i = 0;
    while(i < size)
    {
	if      (bit_Test(c_string[i], 0xfc)) i += 6;
	else if (bit_Test(c_string[i], 0xf8)) i += 5;
	else if (bit_Test(c_string[i], 0xf0)) i += 4;
	else if (bit_Test(c_string[i], 0xe0)) i += 3;
	else if (bit_Test(c_string[i], 0xc0)) i += 2;
	else                                  ++i;
	++length;
    }
    return length;
}

size_t make_index(const char c_string[], size_t length)
{
    if (!length) return 0;
    int i = 0;
    do {
	if      (bit_Test(c_string[i], 0xfc)) i += 6;
	else if (bit_Test(c_string[i], 0xf8)) i += 5;
	else if (bit_Test(c_string[i], 0xf0)) i += 4;
	else if (bit_Test(c_string[i], 0xe0)) i += 3;
	else if (bit_Test(c_string[i], 0xc0)) i += 2;
	else                                  ++i;
    } while(--length);
    return i;
}

struct string *make_string_2(char *c_string, const size_t size, const size_t length)
{
    struct string *str = malloc(sizeof(struct string));

    str->c_string = c_string;
    str->size	  = size;
    str->length   = length;
    return str;
}

struct string *make_string(const char c_string[], const size_t size)
{ 
    char *c_string_2 = malloc(size - 1);
    memcpy(c_string_2, c_string, size - 1);

    return make_string_2(c_string_2, size - 1, make_length(c_string, size - 1));
}

void destroy_string(struct string *str)
{
    free(str->c_string);
    free(str);
}

char *split_2(char *src[], const size_t size, const size_t i)
{
    char *dest = malloc(size - i);
    memcpy(dest, *src + i, size - i);

    *src = realloc(*src, i);
    return dest;
}

struct string *split(struct string *src, const size_t u)
{
    int i = make_index(src->c_string, u);

    struct string *str	= make_string_2(split_2(&(src->c_string), src->size, i), src->size - i, src->length - u);

    src->size	= i;
    src->length	= u;

    return str;
}


void append_2(char **dest, const char *src, const size_t d_size, const size_t s_size)
{
    *dest = realloc(*dest, d_size + s_size);
    memcpy(*dest + d_size, src, s_size);
}

void append(struct string *dest, const struct string *src)
{
    append_2(&(dest->c_string), src->c_string, dest->size, src->size);
    dest->size	   += src->size;
    dest->length   += src->length;
}

void insert(struct string *dest, const struct string *src, const size_t u)
{
    int i = make_index(dest->c_string, u);
    char *half = split_2(&(dest->c_string), dest->size, i);

    append_2(&(dest->c_string), src->c_string, i, src->size);
    append_2(&(dest->c_string), half, i + src->size, dest->size - i);

    dest->length += dest->length;
    dest->size	 += src->size; 

    free(half);
}

int persistent_read(int fd, char *buffer, size_t length)
{
    int n = 0;
    int _;
    while ((_ = read(fd, buffer, length)))
	n += _;
    return n;
}

struct string *make_string_from_read(int fd, size_t n)
{
    char *c_string = malloc(n);
    n = persistent_read(fd, c_string, n);
    c_string = realloc(c_string, n);

    return make_string_2(c_string, n, make_length(c_string, n));
}

int persistent_write(int fd, char *buffer, size_t length)
{
    int _;
    size_t __ = length;
	while ((_ = write(fd, buffer, length)) && (length > 0)) length -= _;
    return length - __;
}

int write_from_string(int fd, struct string *src)
{
    return persistent_write(fd, src->c_string, src->size) ;
}   
