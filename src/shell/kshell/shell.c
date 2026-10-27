#include <stddef.h>
#include <kernel/kernel_load.h>
#include <io/stdio.h>
#include <qemu/io.h>
#include <cpu/control.h>
#include <etc/sysinfo/sysinfo.h>
#include <etc/crashtest/crashtest.h>
#include <etc/crashtest/notmyfault/notmyfault.h>
#include <shell/kshell/shell.h>

extern void qemushutdown(void);

enum { SHELL_LINE_CAPACITY = 128 };

const char *shell_prompt = "root@birdos # ";

static char input_line[SHELL_LINE_CAPACITY];
static size_t input_length;

static int command_equals(const char *command) {
    size_t i = 0;

    while (command[i] != '\0' && input_line[i] == command[i]) {
        i++;
    }

    return command[i] == '\0' && input_line[i] == '\0';
}

void shell_init(void) {
    kprint("\n");
    kprint(shell_prompt);
}

void shell_handle_char(char c) {
    if (input_length >= SHELL_LINE_CAPACITY - 1) {
        return;
    }

    input_line[input_length++] = c;
    input_line[input_length] = '\0';
    char text[] = { c, '\0' };
    kprint(text);
}

void shell_handle_backspace(void) {
    if (input_length == 0) {
        return;
    }

    input_line[--input_length] = '\0';
    kprint("\b");
}

void shell_submit_line(void) {
    kprint("\n");

    if (input_length != 0) {
        if (command_equals("help")) {
            kprint("Commands: help, clear/clr, sysinfo, shutdown, crash <type>, notmyfault\n");
        } else if (command_equals("clear") || command_equals("clr")) {
            clear();
        } else if (command_equals("sysinfo") || command_equals("fetch")) {
            print_sysinfo();
        } else if (command_equals("shutdown")) { // No ACPI for you
            qemushutdown();
        } else if (command_equals("crash div0")) { // first time seeing this HTML tag <div0>, I thought it was <div>
            divide_by_zero();
        } else if (command_equals("crash ud")) {
            invalid_opcode();
        } else if (command_equals("crash bp")) {
            breakpoint();
        } else if (command_equals("crash df")) {
            double_fault();
        } else if (command_equals("notmyfault")) {
            kprint("\nOopsie, not my fault!! >_<"); 
            // You are a god if you manage to see that text
            qemu_console_write("\nOopsie, not my fault!! >_<"); 
            // You are not a god if you manage to see that text,
            // you are only running this in QEMU with the debug
            // console
            run_random_crashtest();
        } else if (command_equals("linux")) {
            kprint("Creator of Linux (i mean the fake BirdOS): linuk towards");
            // git commit -m "fixed typo in linuk toward's name"

            // Not gonna accept a PR on this from Torvalds

            // No offense, Linus (if you're reading this), Linux is such a good OS <3
        } else {
            kprint("Unknown command: "); // If you don't install BirdOS now, you are an unknown command
            kprint(input_line);
            kprint("\n");
        }
    }

    input_length = 0;
    input_line[0] = '\0';
    kprint(shell_prompt);
}