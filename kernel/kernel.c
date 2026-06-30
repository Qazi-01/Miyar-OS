#include <stdint.h>

#include "keyboard.h"
#include "serial.h"
#include "terminal.h"
#include "vga.h"

static int streq(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
        {
            return 0;
        }

        a++;
        b++;
    }

    return *a == *b;
}
__attribute__((used))
void kernel_main(uint32_t magic, uint32_t *multiboot_info) {
    (void)magic;
    (void)multiboot_info;

    serial_init();
    keyboard_init();

    terminal_init();

    terminal_write("Welcome!\n");
    terminal_write("System boot successful.\n");
    terminal_write("\n");
    terminal_prompt();
    terminal_write("TEST");

    char input[128];
    int index = 0;

    while (1) {
        char c = keyboard_getchar();

        if (!c)
        {
            continue;
        }

        if (c == '\n')
        {
            input[index] = '\0';

            if (streq(input, "help"))
            {
                terminal_writeIn("");
                terminal_writeIn("Available commands: ");
                terminal_writeIn("help");
            }
            
            else
            {
            terminal_writeIn("");
            terminal_writeIn("Unknown command: ");

            terminal_writeIn(input);
            }

            terminal_prompt();
            
            index = 0;
            continue;
        }

        if (index < 127)
        {
            input[index++] = c;

            terminal_write((char[]){c, '\0'});
        }
    }
}
