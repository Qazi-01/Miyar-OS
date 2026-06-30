#include "shell.h"
#include "terminal.h"

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

void shell_execute(const char *input)
{
    if (streq(input, "help"))
    {
        terminal_writeIn("");
        terminal_writeIn("Available commands: ");
        terminal_writeIn("help");
    }

    else
    {
        terminal_writeIn("");
        terminal_writeIn("Unknown command: ")
        terminal_writeIn(input);
    }
}