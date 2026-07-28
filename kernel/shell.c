#include "shell.h"
#include "drivers/timer.h"
#include "terminal.h"
#include "drivers/io.h"
#include "drivers/disk.h"
#include "fs/directory.h"
#include "fs/file.h"
#include "fs/path.h"
#include "lib/string.h"

static void trigger_divide_error(void)
{
    __asm__ volatile("xor %%eax, %%eax; div %%eax" ::: "eax");
}

static void trigger_page_fault(void)
{
    volatile uint32_t *ptr = (volatile uint32_t *)0xFFFFFFFF;
    *ptr = 123;
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

static void split_command(const char *input, char *command, char *args)
{
    while (*input && *input != ' ')
    {
        *command++ = *input++;
    }

    *command = '\0';

    if (*input == ' ')
    {
        input++;
    }

    while (*input)
    {
        *args++ = *input++;
    }

    *args = '\0';
}

typedef void (*command_func_t)(const char *args);

struct shell_command
{
    const char *name;
    command_func_t handler;
};

static void cmd_help(const char *args)
{
    (void)args;

    terminal_writeIn("");
    terminal_writeIn("Available commands: ");
    terminal_writeIn("-------------------\n");
    terminal_writeIn("  help            Shows this help message");
    terminal_writeIn("  about           About MiyarOS");
    terminal_writeIn("  echo <msg>      Print text");
    terminal_writeIn("  ls              List files in the current directory");
    terminal_writeIn("  touch <file>    Create a new file");
    terminal_writeIn("  cat <file>      Display the contents of a file");
    terminal_writeIn("  rm <file>       Delete a file");
    terminal_writeIn("  mkdir <dir>     Create a new directory");
    terminal_writeIn("  rmdir <dir>     Remove an empty directory");
    terminal_writeIn("  uptime          Shows system uptime");
    terminal_writeIn("  clear           Clear the screen");
    terminal_writeIn("  reboot          Restart the system");
    terminal_writeIn("  shutdown        Halt the system");
    terminal_writeIn("");
}

static void cmd_about(const char *args)
{
    (void)args;

    terminal_writeIn("");
    terminal_writeIn("MiyarOS v0.2");
    terminal_writeIn("Kernel: 0.2");
    terminal_writeIn("Architecture: x86(32-bits)");
    terminal_writeIn("");
    terminal_writeIn("A hobby operating system written from scratch");
    terminal_writeIn("in C and x86 Assembly.");
    terminal_writeIn("");
    terminal_writeIn("Author: Tashfeen Miyar");
    terminal_writeIn("License: GNU General Public License V3.0(GPLv3)");
    terminal_writeIn("");
}

static void cmd_echo(const char *args)
{
    terminal_writeIn(args);
}

static void cmd_clear(const char *args)
{
    (void)args;
    terminal_clear();
}

static void cmd_ls(const char *args)
{
    (void)args;
    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    directory_t dir;
    fat32_directory_entry_t entry;

    if (!directory_open_root(disk, &dir))
    {
        terminal_writeIn("Unable to open root directory.");
        return;
    }

    while (directory_next(&dir, &entry))
    {
        char filename[13];
        directory_get_name(&entry, filename);
        terminal_writeIn(filename);
    }
}

static void cmd_touch(const char *args)
{
    if (*args == '\0')
    {
        terminal_writeIn("Usage: touch <file>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    if (!file_create(disk, args))
    {
        terminal_writeIn("Unable to create file.");
        return;
    }
}

static void cmd_cat(const char *args)
{
    if (*args == '\0')
    {
        terminal_writeIn("Usage: cat <file>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    fat32_directory_entry_t entry;

    if (!path_resolve_root(disk, args, &entry))
    {
        terminal_writeIn("File not found.");
        return;
    }

    file_t file;

    if (!file_open(disk, &entry, &file))
    {
        terminal_writeIn("Unable to open file.");
        return;
    }

    char buffer[513];

    int bytes = file_read(&file, buffer, sizeof(buffer) - 1);

    if (bytes < 0)
    {
        terminal_writeIn("Read failed.");
        return;
    }

    buffer[bytes] = '\0';

    terminal_writeIn("");
    terminal_write(buffer);
    terminal_writeIn("");
}

static void cmd_mkdir(const char *args)
{
    if (*args == '\0')
    {
        terminal_writeIn("Usage: mkdir <directory>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    if (!directory_create(disk, args))
    {
        terminal_writeIn("Unable to create directory.");
        return;
    }
}

static void cmd_rm(const char *args)
{
    if (*args == '\0')
    {
        terminal_writeIn("Usage: rm <file>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    if (!file_delete(disk, args))
    {
        terminal_writeIn("Unable to delete file.");
        return;
    }
}

static void cmd_rmdir(const char *args)
{
    if (*args == '\0')
    {
        terminal_writeIn("Usage: rmdir <directory>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    if (!directory_remove(disk, args))
    {
        terminal_writeIn("Unable to delete directory.");
        return;
    }
}

static void cmd_uptime(const char *args)
{
    (void)args;

    uint32_t seconds = timer_ticks() / 100;

    write_uint32(seconds);
    terminal_writeIn(" seconds");
}

static void cmd_reboot(const char *args)
{
    (void)args;
    reboot_system();
}

static void cmd_shutdown(const char *args)
{
    (void)args;
    shutdown_system();
}

static void cmd_exception(const char *args)
{
    (void)args;
    trigger_divide_error();
}

static void cmd_pagefault(const char *args)
{
    (void)args;
    trigger_page_fault();
}

static const struct shell_command command_table[] =
    {
        {"help", cmd_help},
        {"about", cmd_about},
        {"echo", cmd_echo},
        {"clear", cmd_clear},
        {"ls", cmd_ls},
        {"touch", cmd_touch},
        {"cat", cmd_cat},
        {"mkdir", cmd_mkdir},
        {"rm", cmd_rm},
        {"rmdir", cmd_rmdir},
        {"exception", cmd_exception},
        {"pagefault", cmd_pagefault},
        {"reboot", cmd_reboot},
        {"shutdown", cmd_shutdown},
        {"uptime", cmd_uptime}};

#define COMMAND_COUNT (sizeof(command_table) / sizeof(command_table[0]))

void shell_execute(const char *input)
{
    if (*input == '\0')
    {
        return;
    }

    char command[32];
    char args[224];

    split_command(input, command, args);

    terminal_writeIn("");

    for (uint32_t i = 0; i < COMMAND_COUNT; i++)
    {
        if (streq(command, command_table[i].name))
        {
            command_table[i].handler(args);
            return;
        }
    }

    terminal_write("Unknown command: ");
    terminal_writeIn(input);
    terminal_writeIn("");
    terminal_writeIn("Type \"help\" for a list of available commands.");
    terminal_writeIn("");
}