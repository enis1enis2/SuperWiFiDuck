/*
   Windows Turkish Q Layout
   Corrected for WiFiDuck HID system
*/

#pragma once
#include "usb_hid_keys.h"


// ================= ASCII TABLE =================
// Modifier, Key (0–127)
const uint8_t ascii_tr[] = {

    // --- 0–31 Control ---
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_BACKSPACE, KEY_NONE,KEY_TAB, KEY_NONE,KEY_ENTER,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,
    KEY_NONE,KEY_NONE, KEY_NONE,KEY_NONE,

    // 32–47
    KEY_NONE,KEY_SPACE,              // space
    KEY_MOD_LSHIFT,KEY_1,            // !
    KEY_MOD_LSHIFT,KEY_2,            // "
    KEY_MOD_LSHIFT,KEY_3,            // #
    KEY_MOD_LSHIFT,KEY_4,            // $
    KEY_MOD_LSHIFT,KEY_5,            // %
    KEY_MOD_LSHIFT,KEY_6,            // &
    KEY_NONE,KEY_2,                  // '

    KEY_MOD_LSHIFT,KEY_8,            // (
    KEY_MOD_LSHIFT,KEY_9,            // )
    KEY_MOD_LSHIFT,KEY_MINUS,        // *
    KEY_MOD_LSHIFT,KEY_4,            // +
    KEY_NONE,KEY_COMMA,              // ,
    KEY_NONE,KEY_SLASH,              // -
    KEY_MOD_LSHIFT,KEY_COMMA,        // .
    KEY_NONE,KEY_7,                  // /

    // 48–57 (0–9)
    KEY_NONE,KEY_0,
    KEY_NONE,KEY_1,
    KEY_NONE,KEY_2,
    KEY_NONE,KEY_3,
    KEY_NONE,KEY_4,
    KEY_NONE,KEY_5,
    KEY_NONE,KEY_6,
    KEY_NONE,KEY_7,
    KEY_NONE,KEY_8,
    KEY_NONE,KEY_9,

    // 58–63
    KEY_MOD_LSHIFT,KEY_DOT,          // :
    KEY_MOD_LSHIFT,KEY_COMMA,        // ;
    KEY_NONE,KEY_102ND,              // <
    KEY_NONE,KEY_0,                  // =
    KEY_MOD_LSHIFT,KEY_102ND,        // >
    KEY_MOD_LSHIFT,KEY_7,            // ?

    // 64
    KEY_MOD_RALT,KEY_Q,              // @

    // A–Z
    KEY_MOD_LSHIFT,KEY_A,
    KEY_MOD_LSHIFT,KEY_B,
    KEY_MOD_LSHIFT,KEY_C,
    KEY_MOD_LSHIFT,KEY_D,
    KEY_MOD_LSHIFT,KEY_E,
    KEY_MOD_LSHIFT,KEY_F,
    KEY_MOD_LSHIFT,KEY_G,
    KEY_MOD_LSHIFT,KEY_H,
    KEY_MOD_LSHIFT,KEY_I,
    KEY_MOD_LSHIFT,KEY_J,
    KEY_MOD_LSHIFT,KEY_K,
    KEY_MOD_LSHIFT,KEY_L,
    KEY_MOD_LSHIFT,KEY_M,
    KEY_MOD_LSHIFT,KEY_N,
    KEY_MOD_LSHIFT,KEY_O,
    KEY_MOD_LSHIFT,KEY_P,
    KEY_MOD_LSHIFT,KEY_Q,
    KEY_MOD_LSHIFT,KEY_R,
    KEY_MOD_LSHIFT,KEY_S,
    KEY_MOD_LSHIFT,KEY_T,
    KEY_MOD_LSHIFT,KEY_U,
    KEY_MOD_LSHIFT,KEY_V,
    KEY_MOD_LSHIFT,KEY_W,
    KEY_MOD_LSHIFT,KEY_X,
    KEY_MOD_LSHIFT,KEY_Y,
    KEY_MOD_LSHIFT,KEY_Z,

    // 91–96
    KEY_MOD_RALT,KEY_8,              // [
    KEY_MOD_RALT,KEY_MINUS,          // \
    KEY_MOD_RALT,KEY_9,              // ]
    KEY_MOD_LSHIFT,KEY_3,            // ^
    KEY_MOD_LSHIFT,KEY_SLASH,        // _
    KEY_NONE,KEY_APOSTROPHE,         // `

    // a–z
    KEY_NONE,KEY_A,
    KEY_NONE,KEY_B,
    KEY_NONE,KEY_C,
    KEY_NONE,KEY_D,
    KEY_NONE,KEY_E,
    KEY_NONE,KEY_F,
    KEY_NONE,KEY_G,
    KEY_NONE,KEY_H,
    KEY_NONE,KEY_I,
    KEY_NONE,KEY_J,
    KEY_NONE,KEY_K,
    KEY_NONE,KEY_L,
    KEY_NONE,KEY_M,
    KEY_NONE,KEY_N,
    KEY_NONE,KEY_O,
    KEY_NONE,KEY_P,
    KEY_NONE,KEY_Q,
    KEY_NONE,KEY_R,
    KEY_NONE,KEY_S,
    KEY_NONE,KEY_T,
    KEY_NONE,KEY_U,
    KEY_NONE,KEY_V,
    KEY_NONE,KEY_W,
    KEY_NONE,KEY_X,
    KEY_NONE,KEY_Y,
    KEY_NONE,KEY_Z,

    KEY_MOD_RALT,KEY_7,              // {
    KEY_MOD_RALT,KEY_102ND,          // |
    KEY_MOD_RALT,KEY_0,              // }
    KEY_MOD_LSHIFT,KEY_APOSTROPHE,   // ~
    KEY_NONE,KEY_DELETE
};


// ================= UTF-8 TURKISH LETTERS =================

const uint8_t utf8_tr[] = {

    // ç Ç
    0xC3,0xA7, KEY_NONE,KEY_DOT, 0,0,
    0xC3,0x87, KEY_MOD_LSHIFT,KEY_DOT, 0,0,

    // ğ Ğ
    0xC4,0x9F, KEY_NONE,KEY_LEFTBRACE, 0,0,
    0xC4,0x9E, KEY_MOD_LSHIFT,KEY_LEFTBRACE, 0,0,

    // ö Ö
    0xC3,0xB6, KEY_NONE,KEY_SEMICOLON, 0,0,
    0xC3,0x96, KEY_MOD_LSHIFT,KEY_SEMICOLON, 0,0,

    // ş Ş
    0xC5,0x9F, KEY_NONE,KEY_APOSTROPHE, 0,0,
    0xC5,0x9E, KEY_MOD_LSHIFT,KEY_APOSTROPHE, 0,0,

    // ü Ü
    0xC3,0xBC, KEY_NONE,KEY_LEFTBRACE, 0,0,
    0xC3,0x9C, KEY_MOD_LSHIFT,KEY_LEFTBRACE, 0,0,

    // ı İ
    0xC4,0xB1, KEY_NONE,KEY_I, 0,0,
    0xC4,0xB0, KEY_MOD_LSHIFT,KEY_I, 0,0,
};

const uint8_t combinations_tr[] = {
};

static hid_locale_t locale_tr = {
    (uint8_t*)ascii_tr, 128,
    (uint8_t*)utf8_tr, sizeof(utf8_tr) / 6,
    (uint8_t*)combinations_tr, sizeof(combinations_tr) / 8,
};
