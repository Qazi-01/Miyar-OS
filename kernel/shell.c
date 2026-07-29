#include "shell.h"
#include "drivers/timer.h"
#include "terminal.h"
#include "drivers/io.h"
#include "drivers/disk.h"
#include "fs/directory.h"
#include "fs/file.h"
#include "fs/path.h"
#include "lib/string.h"
#include "fs/fs.h"

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
    uint32_t i = 0;

    while (*input && *input != ' ' && i < 31)
    {
        command[i++] = *input++;
    }

    command[i] = '\0';

    if (*input == ' ')
    {
        input++;
    }

    uint32_t j = 0;
    while (*input && j < 223)
    {
        args[j++] = *input++;
    }

    args[j] = '\0';
}

static bool get_next_argument(const char **input, char *output, uint32_t size)
{
    if (input == 0 || *input == 0 || output == 0)
    {
        return false;
    }

    while (**input == ' ')
    {
        (*input)++;
    }

    if (**input == '\0')
    {
        return false;
    }

    uint32_t i = 0;

    while (**input && **input != ' ' && i < size - 1)
    {
        output[i++] = **input;
        (*input)++;
    }

    output[i] = '\0';

    return true;
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
    terminal_writeIn("  help                    Shows this help message");
    terminal_writeIn("  about                   About MiyarOS");
    terminal_writeIn("  echo <msg>              Print text");
    terminal_writeIn("             > <file>     Write text to a file");
    terminal_writeIn("             >> <file>    Append text to a file");
    terminal_writeIn("  ls                      List files in the current directory");
    terminal_writeIn("  cd <dir>                Change current directory");
    terminal_writeIn("  pwd                     Print current working directory");
    terminal_writeIn("  touch <file>            Create a new file");
    terminal_writeIn("  cat <file>              Display the contents of a file");
    terminal_writeIn("  rm <file>               Delete a file");
    terminal_writeIn("  cp <src> <dest>         Copy a file");
    terminal_writeIn("  mv <src> <dest>         Move or rename a file");
    terminal_writeIn("  mkdir <dir>             Create a new directory");
    terminal_writeIn("  rmdir <dir>             Remove an empty directory");
    terminal_writeIn("  uptime                  Shows system uptime");
    terminal_writeIn("  clear                   Clear the screen");
    terminal_writeIn("  reboot                  Restart the system");
    terminal_writeIn("  shutdown                Halt the system");
    terminal_writeIn("");
}

static void cmd_about(const char *args)
{
    (void)args;

    terminal_writeIn("");
    terminal_writeIn("MiyarOS v0.3.0");
    terminal_writeIn("Kernel: 0.3.0");
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
    const char *redirect = 0;
    bool append = false;

    for (const char *p = args; *p; p++)
    {
        if (*p == '>')
        {
            redirect = p;

            if (*(p + 1) == '>')
            {
                append = true;
            }

            break;
        }
    }

    if (redirect == 0)
    {
        terminal_writeIn(args);
        return;
    }

    char text[256];
    char filename[128];

    uint32_t text_length = (uint32_t)(redirect - args);

    while (text_length > 0 && args[text_length - 1] == ' ')
    {
        text_length--;
    }

    memcpy(text, args, text_length);
    text[text_length] = '\0';

    const char *name = redirect + (append ? 2 : 1);

    while (*name == ' ')
    {
        name++;
    }

    uint32_t i = 0;

    while (*name && i < sizeof(filename) - 1)
    {
        filename[i++] = *name++;
    }

    filename[i] = '\0';

    if (filename[0] == '\0')
    {
        terminal_writeIn("Usage: echo <text> > <file>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    fat32_directory_entry_t entry;

    if (!directory_find(disk, filename, &entry))
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

    int written;

    if (append)
    {
        written = file_append(&file, text, strlen(text));
    }
    else
    {
        written = file_write(&file, text, strlen(text));
    }

    if (written < 0)
    {
        terminal_writeIn("Write failed.");
    }
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

    char filename[64];

    while (get_next_argument(&args, filename, sizeof(filename)))
    {
        if (!file_create(disk, filename))
        {
            terminal_write("Failed: ");
            terminal_writeIn(filename);
        }
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

    char dirname[64];

    while (get_next_argument(&args, dirname, sizeof(dirname)))
    {
        if (!directory_create(disk, dirname))
        {
            terminal_write("Failed: ");
            terminal_writeIn(dirname);
        }
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

    char filename[64];

    while (get_next_argument(&args, filename, sizeof(filename)))
    {
        if (!file_delete(disk, filename))
        {
            terminal_write("Failed: ");
            terminal_writeIn(filename);
        }
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

    char dirname[64];

    while (get_next_argument(&args, dirname, sizeof(dirname)))
    {
        if (!directory_delete(disk, dirname))
        {
            terminal_write("Failed: ");
            terminal_writeIn(dirname);
        }
    }
}

static void cmd_cp(const char *args)
{
    char source[128];
    char destination[128];

    uint32_t i = 0;

    while (*args && *args != ' ' && i < sizeof(source) - 1)
    {
        source[i++] = *args++;
    }

    source[i] = '\0';

    while (*args == ' ')
    {
        args++;
    }

    i = 0;

    while (*args && i < sizeof(destination) - 1)
    {
        destination[i++] = *args++;
    }

    destination[i] = '\0';

    if (source[0] == '\0' || destination[0] == '\0')
    {
        terminal_writeIn("Usage: cp <source> <destination>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    if (!file_copy(disk, source, destination))
    {
        terminal_writeIn("Copy failed.");
    }
}

static void cmd_mv(const char *args)
{
    char old_name[128];
    char new_name[128];

    uint32_t i = 0;

    while (*args && *args != ' ' && i < sizeof(old_name) - 1)
    {
        old_name[i++] = *args++;
    }

    old_name[i] = '\0';

    while (*args == ' ')
    {
        args++;
    }

    i = 0;

    while (*args && i < sizeof(new_name) - 1)
    {
        new_name[i++] = *args++;
    }

    new_name[i] = '\0';

    if (old_name[0] == '\0' || new_name[0] == '\0')
    {
        terminal_writeIn("Usage: mv <old> <new>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    fat32_directory_entry_t entry;

    if (!directory_find(disk, old_name, &entry))
    {
        terminal_writeIn("File not found.");
        return;
    }

    if (!file_move(disk, old_name, new_name))
    {
        terminal_writeIn("Move failed.");
    }
}

static void cmd_pwd(const char *args)
{
    (void)args;

    terminal_writeIn(fs_get_current_path());
}

static void cmd_cd(const char *args)
{
    if (*args == '\0')
    {
        terminal_writeIn("Usage: cd <directory>");
        return;
    }

    const disk_t *disk = disk_get(0);

    if (disk == 0)
    {
        terminal_writeIn("No disk available.");
        return;
    }

    if (!directory_change(disk, args))
    {
        terminal_writeIn("Directory not found.");
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
        {"cp", cmd_cp},
        {"mv", cmd_mv},
        {"rm", cmd_rm},
        {"rmdir", cmd_rmdir},
        {"pwd", cmd_pwd},
        {"cd", cmd_cd},
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
        terminal_writeIn("");
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