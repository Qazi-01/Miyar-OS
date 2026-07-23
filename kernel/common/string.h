#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, int value, size_t count);
int memcmp(const void *a, const void *b, size_t count);

size_t strlen(const char *str);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, size_t count);

#endif