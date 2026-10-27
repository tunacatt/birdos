#include <stdbool.h>

#ifndef KB_LAYOUT
#define KB_LAYOUT

typedef struct {
    const char *unshifted;
    const char *shifted;
} Layout;
extern const Layout layout_us_qwerty;

typedef enum {
    // ---
    KEY_NONE = 0,

    // Letter keys
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    // Number keys
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3, // I didnt know, this really is 3! WoW!
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    // Punctuation keys
    KEY_MINUS,
    KEY_EQUAL,
    KEY_COMMA,
    KEY_DOT,
    KEY_SLASH,
    KEY_LEFTBRACKET,
    KEY_RIGHTBRACKET,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_GRAVE,
    KEY_BACKSLASH,
    KEY_SPACE,

    // Non-char keys
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_ENTER,
    KEY_ESC,
    KEY_CAPS,
    KEY_COUNT,
    // ... 
} KeyCode;

void kblayout_handle_key_event(KeyCode ecode, bool epressed);
static char kblayout_get_char(KeyCode code, bool pressed);

#endif