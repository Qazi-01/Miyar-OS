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

static int starts_with(const char *str, const char *prefix)
{
    while (*prefix)
    {
        if (*str != *prefix)
        {
            return 0;
        }

        str++;
        prefix++;
    }

    return 1;
}

void shell_execute(const char *input)
{
    terminal_writeIn("");

    if (streq(input, "help"))
    {
        terminal_writeIn("Available commands: ");
        terminal_writeIn("help");
        terminal_writeIn("about");
        terminal_writeIn("version");
        terminal_writeIn("echo <message>");
        terminal_writeIn("clear");
    }

    else if (streq(input, "about"))
    {
        terminal_writeIn("MiyarOS");
        terminal_writeIn("A hobby operating system written in C");
    }

    else if (streq(input, "version"))
    {
        terminal_writeIn("MiyarOS v0.1");
    }

    else if (starts_with(input, "echo "))
    {
        terminal_writeIn(input + 5);
    }

    else if (streq(input, "clear"))
    {
        terminal_clear();
    }

    else
    {
        terminal_write("Unknown command: ");
        terminal_writeIn(input);
    }
}