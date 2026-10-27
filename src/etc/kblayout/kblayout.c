#include <io/stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <etc/kblayout/kblayout.h>
#include <shell/kshell/shell.h>

static bool KEY_CAPS_ENABLED = false;
static bool KEY_SHIFT_HELD = false;

static bool is_letter(KeyCode code) {
    return (code >= KEY_A && code <= KEY_Z);
}

static char kblayout_get_char(KeyCode code, bool pressed) {
    if (code >= KEY_COUNT) {
        return '\0'; // Invalid key code
    }

    bool use_shifted = is_letter(code) ? (KEY_SHIFT_HELD ^ KEY_CAPS_ENABLED) : KEY_SHIFT_HELD;

    return use_shifted ? layout_us_qwerty.shifted[code] : layout_us_qwerty.unshifted[code];

    (void)pressed;
}

void kblayout_handle_special_key(KeyCode code, bool pressed) {
    switch (code) {
        case KEY_ENTER:
            kprint("\n"); kprint(shell_prompt);
            break;
    }
}

void kblayout_handle_key_event(KeyCode code, bool pressed) {
    switch (code)
    {
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            KEY_SHIFT_HELD = pressed;
            return;
        case KEY_CAPS:
            if (pressed) KEY_CAPS_ENABLED = !KEY_CAPS_ENABLED;
            return;
        default:
            break;
    }

    if (!pressed) return;

    switch (code) {
        case KEY_ENTER:
            shell_submit_line();
            break;
        case KEY_BACKSPACE:
            shell_handle_backspace();
            break;
        case KEY_ESC:
        case KEY_TAB:
            // Handle other non-character keys if needed
            break;
        default: {
            char c = kblayout_get_char(code, pressed);
            if (c) {
                shell_handle_char(c);
            }
            break;
        }
    }
}
