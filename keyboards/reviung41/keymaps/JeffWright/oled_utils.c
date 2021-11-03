#include "status.h"
#include "keycodes.h"
#include "casemodes.h"
//#include "layermodes.h"
#include <stdio.h>

void render_layers(void) {
    //oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("BASE\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("SYM\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("NUM\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("NAV\n"), false);
            break;
        case _OPT:
            oled_write_P(PSTR("OPT\n"), false);
            break;
        case _FNKEYS:
            oled_write_P(PSTR("FNKEY\n"), false);
            break;
        default:
            oled_write_P(PSTR("???\n"), false);
    }
}

void render_mod(uint8_t mods, uint8_t os_mods, uint8_t mask, char mod, char os) {
    if (mods & mask) {
        oled_write_char(mod, false);
        //oled_write_char(' ', false);
    } else if (os_mods & mask) {
        oled_write_char(os, false);
       //oled_write_char(' ', false);
    } else {
        oled_write_P(PSTR(" "), false);
    }
}

void render_mods(void) {
    uint8_t mods = get_mods();
    uint8_t os_mods = get_oneshot_mods();

    oled_write_P(PSTR("Mods: "), false);
    oled_write_ln("", false);
    render_mod(mods, os_mods, MOD_MASK_GUI, 'G', 'g');
    render_mod(mods, os_mods, MOD_MASK_ALT, 'A', 'a');
    render_mod(mods, os_mods, MOD_MASK_CTRL, 'C', 'c');
    render_mod(mods, os_mods, MOD_MASK_SHIFT, 'S', 's');

    oled_write_ln("", false);
}

void render_nix(void) {
    oled_write_P(in_linux() ? PSTR("Linux") : PSTR("Mac"), false);
    oled_write_ln("", false);
}

void render_caps(void) {
    bool caps_word = caps_word_enabled();
    //bool xcase = xcase_enabled();
    bool xcase = get_xcase_state() !=XCASE_OFF;
    if (caps_word && xcase) {
        oled_write_P(PSTR("U_CPS"), false);
    } else if (xcase) {
        oled_write_P(PSTR("XCASE"), false);
    } else if (caps_word) {
        oled_write_P(PSTR("CAPWD"), false);
    } else if (host_keyboard_led_state().caps_lock) {
        oled_write_P(PSTR("CAPS"), false);
    }
    oled_write_ln("", false);
}

void render_master_status(void) {
    render_layers();
    oled_write_ln("", false);
    render_mods();
    oled_write_ln("", false);
    render_nix();
    oled_write_ln("", false);
    render_caps();
    oled_write_ln("", false);
}

void render_slave_status(void) {
}

void render_status(void) {
    if (is_keyboard_master()) {
        render_master_status();
    } else {
        /* render_slave_status(); */
    }
}
