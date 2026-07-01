#ifndef TERMINAL_H
#define TERMINAL_H

void terminal_clear(void);
void terminal_init(void);
void terminal_write(const char *str);
void terminal_writeIn(const char *str);
void terminal_prompt(void);

#endif