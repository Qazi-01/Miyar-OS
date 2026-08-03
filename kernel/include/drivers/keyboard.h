#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

char keyboard_getchar(void);
void keyboard_init(void);
void keyboard_buffer_put(char c);

#endif
