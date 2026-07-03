#include "shell.h"
#include "timer.h"
#include "terminal.h"
#include "io.h"

static void trigger_divide_error(void)
{
    __asm__ volatile("xor %%eax, %%eax; div %%eax" ::: "eax");
}

static void halt_forever(void)
{
    __asm__ volatile("cli");

    while (1)
    {
        __asm__ volatile("hlt");
    }
}

static void reboot_system(void)
{
    terminal_writeIn("Rebooting...");

    struct
    {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) empty_idt = {0, 0};

    __asm__ volatile("cli");
    __asm__ volatile("lidt %0" : : "m"(empty_idt));
    __asm__ volatile("int $0x3");

    halt_forever();
}

static void shutdown_system(void)
{
    terminal_writeIn("Shutting down...");
    outw(0x604, 0x2000);

    halt_forever();
}

static void write_uint32(uint32_t value)
{
    char buffer[11];
    int index = 0;

    if (value == 0)
    {
        terminal_writeIn("0");
        return;
    }

    while (value > 0 && index < 10)
    {
        buffer[index++] = (char)('0' + (value % 10));
        value /= 10;
    }

    while (index > 0)
    {
        index--;
        terminal_write((char[]){buffer[index], '\0'});
    }
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
        terminal_writeIn("reboot");
        terminal_writeIn("shutdown");
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

    else if (streq(input, "shutdown"))
    {
        shutdown_system();
    }

    else if (streq(input, "uptime"))
    {
        uint32_t seconds = timer_ticks() / 100;

        terminal_write("Uptime: ");
        write_uint32(seconds);
        terminal_writeIn(" seconds");
    }

    else
    {
        terminal_write("Unknown command: ");
        terminal_writeIn(input);
    }
}