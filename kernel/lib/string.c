#include "string.h"

void *memcpy(void *dest, const void *src, size_t count)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    while (count--)
    {
        *d++ = *s++;
    }

    return dest;
}

void *memset(void *dest, int value, size_t count)
{
    unsigned char *d = (unsigned char *)dest;

    while (count--)
    {
        *d++ = (unsigned char)value;
    }

    return dest;
}

int memcmp(const void *a, const void *b, size_t count)
{
    const unsigned char *p1 = (const unsigned char *)a;
    const unsigned char *p2 = (const unsigned char *)b;

    while (count--)
    {
        if (*p1 != *p2)
    {
        return *p1 - *p2;
    }
        p1++;
        p2++;
    }

    return 0;
}

size_t strlen(const char *str)
{
    size_t length = 0;

    while (str[length] != '\0')
    {
        length++;
    }

    return length;
}

int strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return (unsigned char)*a - (unsigned char)*b;
}

char *strcpy(char *dest, const char *src)
{
    char *start = dest;

    while ((*dest++ = *src++) != '\0')
    {
    }

    return start;
}

char *strncpy(char *dest, const char *src, size_t count)
{
    char *start = dest;

    while (count && *src)
    {
        *dest++ = *src++;
        count--;
    }

    while (count)
    {
        *dest++ = '\0';
        count--;
    }

    return start;
}

int strncmp(const char *a, const char *b, size_t count)
{
    while (count && *a && (*a == *b))
    {
        a++;
        b++;
        count--;
    }

    if (count == 0)
    {
        return 0;
    }

    return (unsigned char)*a - (unsigned char)*b;
}