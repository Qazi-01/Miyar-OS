#include "shell.h"
#include "terminal.h"
#include "io.h"

static void trigger_divide_error(void)
{
    __asm__ volatile("xor %%eax, %%eax; div %%eax" ::: "eax");
}

static void reboot_system(void)
{
    terminal_writeIn("Rebooting...");
    outb(0x64, 0xFE);
}

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
    if (*input == '\0')
    {
        return;
    }

    terminal_writeIn("");

    if (streq(input, "help"))
    {
        terminal_writeIn("Available commands: ");
        terminal_writeIn("help");
        terminal_writeIn("about");
        terminal_writeIn("version");
        terminal_writeIn("echo <message>");
        terminal_writeIn("uptime");
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

    else if (streq(input, "exception"))
    {
        trigger_divide_error();
    }

    else if (streq(input, "reboot"))
    {
        reboot_system();
    }

    else if (streq(input, "uptime"))
    {
        terminal_writeIn("Uptime: Timer not implemented yet.");
    }

    else
    {
        terminal_write("Unknown command: ");
        terminal_writeIn(input);
    }
}