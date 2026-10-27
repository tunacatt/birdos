#ifndef KSHELL_H
#define KSHELL_H

extern const char *shell_prompt;

void shell_init(void);
void shell_handle_char(char c);
void shell_handle_backspace(void);
void shell_submit_line(void);

#endif