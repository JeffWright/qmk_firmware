/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "keycodes.h"
#include "status.h"
#include "oneshot.h"
#include "casemodes.h"
#include "layermodes.h"
#include "tap_hold.h"

#include "keymap_swedish.h"
#include "sendstring_swedish.h"
#include "g/keymap_combo.h"

#ifdef ENCODER_ENABLE
#    include "encoder_utils.h"
#endif

#ifdef OLED_DRIVER_ENABLE
#    include "oled_utils.h"
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
    * Base Layer: Modified RSTHD
    */
    [_BASE] = LAYOUT(
      xxxxxxx, SE_Y,    SE_C,    SE_K,    SE_F,    SE_J,                                        SE_X,    SE_W,    SE_DOT,  SE_U,    SE_COMM, xxxxxxx,
      xxxxxxx, SE_R,    SE_S,    SE_T,    SE_H,    SE_P,                                        SE_M,    SE_N,    SE_A,    SE_I,    SE_O,    xxxxxxx,
      xxxxxxx, SE_SLSH, SE_V,    SE_G,    SE_D,    SE_B,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SE_EQL,  SE_L,    SE_LPRN, SE_RPRN, SE_UNDS, xxxxxxx,
                                 xxxxxxx, xxxxxxx, LMOD,    MT_SPC,  xxxxxxx, xxxxxxx, SE_E,    RMOD,    xxxxxxx, KC_MUTE
    ),
    [_SWE] = LAYOUT(
      xxxxxxx, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, SE_ARNG, SE_ADIA, SE_ODIA, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    [_NUM] = LAYOUT(
      xxxxxxx, xxxxxxx, xxxxxxx, SE_K,    SE_PERC, SE_J,                                        xxxxxxx, xxxxxxx, SE_DOT,  xxxxxxx, SE_COMM, xxxxxxx,
      xxxxxxx, SE_6,    SE_4,    SE_0,    SE_2,    MY_000,                                      xxxxxxx, SE_3,    SE_1,    SE_5,    SE_7,    xxxxxxx,
      xxxxxxx, SE_SLSH, xxxxxxx, NUM_G,   SE_8,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SE_EQL,  SE_9,    SE_LPRN, SE_RPRN, SE_UNDS, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, CANCEL,  _______, xxxxxxx, _______
    ),
    [_NAV] = LAYOUT(
      xxxxxxx, SC_TAB,  C_TAB,   KC_UP,   KC_PGUP, KC_HOME,                                     xxxxxxx, G(SE_W), G(SE_E), G(SE_R), xxxxxxx, xxxxxxx,
      xxxxxxx, xxxxxxx, KC_LEFT, DN_CTRL, KC_RGHT, KC_ENT,                                      xxxxxxx, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, xxxxxxx,
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_PGDN, KC_END,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, G_WNAV,  _______, xxxxxxx, _______
    ),
    // Important that the symbols on the base layer have the same positions as these symbols
    [_LMOD] = LAYOUT(
      xxxxxxx, OPT,     C(SE_A), C(SE_W), xxxxxxx, xxxxxxx,                                     xxxxxxx, SE_HASH, SE_DOT,  TILD,    SE_COMM, xxxxxxx,
      xxxxxxx, OS_ALT,  OS_SHFT, OS_CTRL, OS_GUI,  xxxxxxx,                                     SE_AT,   SE_QUES, SE_LBRC, SE_RBRC, SE_AMPR, xxxxxxx,
      xxxxxxx, C(SE_Z), C(SE_X), xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SE_EQL,  SE_EXLM, SE_LPRN, SE_RPRN, SE_UNDS, xxxxxxx,
                                 _______, xxxxxxx, _______, CANCEL,  xxxxxxx, xxxxxxx, CANCEL,  FUN,     xxxxxxx, _______
    ),
    [_RMOD] = LAYOUT(
      xxxxxxx, xxxxxxx, SE_LCBR, SE_RCBR, SE_PERC, xxxxxxx,                                     SE_DIAE, SE_GRV,  SE_CIRC, SE_ACUT, SE_TILD, xxxxxxx,
      xxxxxxx, SE_PIPE, SE_PLUS, SE_ASTR, SE_MINS, GRV,                                         xxxxxxx, OS_GUI,  OS_CTRL, OS_SHFT, OS_ALT,  xxxxxxx,
      xxxxxxx, SE_SLSH, SE_LABK, SE_RABK, SE_BSLS, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, MY_RALT, xxxxxxx,
                                 _______, xxxxxxx, FUN,     CANCEL,  xxxxxxx, xxxxxxx, CANCEL,  _______, xxxxxxx, _______
    ),
    [_WNAV] = LAYOUT(
      xxxxxxx, xxxxxxx, xxxxxxx, SE_K,    xxxxxxx, SE_J,                                        xxxxxxx, SE_W,    SE_E,    SE_R,    xxxxxxx, xxxxxxx,
      xxxxxxx, SE_6,    SE_4,    SE_0,    SE_2,    KC_ENT,                                      xxxxxxx, SE_3,    SE_1,    SE_5,    SE_7,    xxxxxxx,
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SE_8,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SE_9,    SE_H,    SE_L,    xxxxxxx, xxxxxxx,
                                 _______, xxxxxxx, _______, SPC_SFT, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    [_FUN] = LAYOUT(
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,                                     xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
      xxxxxxx, KC_F6,   KC_F4,   KC_F10,  KC_F2,   KC_F12,                                      KC_F11,  KC_F3,   KC_F1,   KC_F5,   KC_F7,   xxxxxxx,
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_F8,   xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_F9,   xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, CLEAR,   xxxxxxx, _______
    ),
    [_OPT] = LAYOUT(
      xxxxxxx, _______, TG_CAPS, _______, _______, _______,                                     _______, TG_NIX,  _______, _______, _______, xxxxxxx,
      xxxxxxx, _______, _______, KC_CAPS, _______, _______,                                     _______, TG_NUM,  _______, _______, _______, xxxxxxx,
      xxxxxxx, _______, _______, TO_GAME, _______, _______, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    [_GAME] = LAYOUT(
      xxxxxxx, KC_ENT,  SE_Q,    SE_W,    SE_E,    SE_R,                                        SE_T,    SE_Y,    SE_U,    SE_I,    SE_O,    xxxxxxx,
      xxxxxxx, KC_LSFT, SE_A,    SE_S,    SE_D,    SE_F,                                        SE_G,    SE_H,    SE_J,    SE_K,    SE_L,    xxxxxxx,
      xxxxxxx, KC_LCTL, SE_Z,    SE_X,    SE_C,    SE_V,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SE_B,    SE_N,    SE_M,    xxxxxxx, xxxxxxx, xxxxxxx,
                                 _______, xxxxxxx, GAME2,   MT_SPC,  xxxxxxx, xxxxxxx, CANCEL,  _______, xxxxxxx, _______
    ),
    [_GAME2] = LAYOUT(
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, SE_1,    SE_2,    SE_3,    SE_4,    SE_5,                                        _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    )
    /*
    [_BLANK] = LAYOUT(
      xxxxxxx, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    */
};

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        // Home-row and other tight combos
        case tab:
        case escape:
        case scln:
        case dquo:
        case coln:
        case enter:
        case quot:
        case circ:
        case dlr:
        case vsp:
            return COMBO_TERM;
        // Vertical combos, very relaxed
        case small_left_arrow:
        case lt_eq:
        case large_right_arrow:
        case small_right_arrow:
        case pipe_to:
        case sp:
        case gt_eq:
            return COMBO_TERM + 55;
        // Regular combos, slightly relaxed
        default:
            return COMBO_TERM + 35;
    }
}

bool get_combo_must_tap(uint16_t index, combo_t *combo) {
    switch (index) {
        case del:
        case backsp:
        case q_comb:
        case z_comb:
        case num:
        //case sym:
        case dquo:
        case sp_ampr:
        case sp_pipe:
        case sp_exlm:
        case sp_plus:
        case sp_astr:
        case sp_mins:
        case sp_unds:
        case sp_perc:
        case sp_grv:
        case sp_labk:
        case sp_rabk:
        case sp_lbrc:
        case sp_lprn:
        case sp_lcbr:
        case sp_bsls:
            return false;
        default:
            return true;
    }
}

static bool linux_mode = true;
bool in_linux(void) {
    return linux_mode;
}

static bool swap_caps_escape = true;
bool is_caps_swapped(void) {
    return swap_caps_escape;
}

bool tap_undead_key(bool key_down, uint16_t code) {
    if (key_down) {
        tap_code16(code);
        tap_code16(KC_SPACE);
    }
    return false;
}

void swap_caps_esc(void) {
    swap_caps_escape = !swap_caps_escape;
#   ifdef OLED_DRIVER_ENABLE
        oled_on();
#   endif
}

void tg_nix(void) {
    linux_mode = !linux_mode;
#   ifdef OLED_DRIVER_ENABLE
        oled_on();
#   endif
}

bool process_caps(bool key_down) {
    if (swap_caps_escape) {
        if (key_down) {
            register_code(KC_ESC);
        } else {
            unregister_code(KC_ESC);
        }
        return false;
    }
    return true;
}

bool process_escape(bool key_down) {
    if (swap_caps_escape) {
        if (key_down) {
            register_code(KC_CAPS);
        } else {
            unregister_code(KC_CAPS);
        }
        return false;
    }
    return true;
}

void tap_escape(void) {
    tap_code(swap_caps_escape ? KC_CAPS : KC_ESC);
}

bool terminate_case_modes(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
        // Keycodes to ignore (don't disable caps word)
        case SE_A ... KC_Z:
        case SE_1 ... KC_0:
        case SE_MINS:
        case SE_UNDS:
        case KC_BSPC:
            // If mod chording disable the mods
            if (record->event.pressed && (get_mods() != 0)) {
                return true;
            }
            break;
        default:
            if (record->event.pressed) {
                return true;
            }
            break;
    }
    return false;
}

bool use_default_xcase_separator(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
        case SE_A ... SE_Z:
        case SE_1 ... SE_0:
            return true;
    }
    return false;
}

void enable_gaming(void) {
    autoshift_disable();
    layer_on(_GAME);
}
void disable_gaming(void) {
    autoshift_enable();
    layer_off(_GAME);
    layer_off(_GAME2);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LMOD, _RMOD, _FUN);
}

void tap_space_shift(uint16_t key, bool key_down) {
    if (key_down) {
        tap_code16(key);
        tap_code(KC_SPC);
        set_oneshot_mods(MOD_BIT(KC_LSFT));
    }
}

void double_tap(uint16_t keycode) {
    tap_code16(keycode);
    tap_code16(keycode);
}
void triple_tap(uint16_t keycode) {
    tap_code16(keycode);
    tap_code16(keycode);
    tap_code16(keycode);
}

void double_parens_left(uint16_t left, uint16_t right) {
    tap_code16(left);
    tap_code16(right);
    tap_code16(KC_LEFT);
}

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case CLEAR:
        case LMOD:
        case RMOD:
            return true;
        default:
            return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case CLEAR:
        case LMOD:
        case RMOD: 
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_GUI:
            return true;
        default:
            return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_num_word(keycode, record)) {
        return false;
    }
    /* if (!process_sym_word(keycode, record)) { */
    /*     return false; */
    /* } */
    if (!process_case_modes(keycode, record)) {
        return false;
    }

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );

    switch (keycode) {
        case KC_ESC:
            return process_escape(record->event.pressed);
        case KC_CAPS:
            return process_caps(record->event.pressed);
        case TG_NIX:
            if (record->event.pressed) {
                tg_nix();
            }
            return false;
        case TG_CAPS:
            if (record->event.pressed) {
                swap_caps_esc();
            }
            return false;
        case CLEAR:
            clear_oneshot_mods();
            if (get_oneshot_layer() != 0) {
                clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            }
            layer_move(_BASE);
            return false;
        case TILD:
            return tap_undead_key(record->event.pressed, SE_TILD);
        case CIRC:
            return tap_undead_key(record->event.pressed, SE_CIRC);
        case MY_000:
            if (record->event.pressed) {
                triple_tap(SE_0);
            }
            return false;
        /* case SYM: */
        /*     process_sym_word_activation(record); */
        /*     return false; */
        case TG_NUM:
            // FIXME doesn't work
            if (layer_state_is(_NUM)) {
                disable_num_word();
            } else {
                layer_on(_NUM);
            }
            return false;
        case NUM:
            process_num_word_activation(record);
            return false;
        case CAPSWORD:
            if (record->event.pressed) {
                enable_caps_word();
            }
            return false;
        case XCASE:
            if (record->event.pressed) {
                enable_xcase();
            }
            return false;
        case SAVE_VIM:
            if (record->event.pressed) {
                tap_escape();
                tap_code16(SE_COLN);
                tap_code(SE_W);
                tap_code(KC_ENT);
            }
            return false;
        case CLOSE_VIM:
            if (record->event.pressed) {
                tap_escape();
                tap_code16(SE_COLN);
                tap_code(SE_Q);
                tap_code(KC_ENT);
            }
            return false;
        case VIM_SP:
            if (record->event.pressed) {
                tap_code16(C(SE_W));
                tap_code(SE_S);
            }
            return false;
        case VIM_VS:
            if (record->event.pressed) {
                tap_code16(C(SE_W));
                tap_code(SE_V);
            }
            return false;
        case NUM_UP:
            if (record->event.pressed) {
                tap_code(KC_UP);
            }
            return false;
        case NUM_DN:
            if (record->event.pressed) {
                tap_code(KC_DOWN);
            }
            return false;
        case NUM_G:
            if (record->event.pressed) {
                tap_code16(S(SE_G));
            }
            return false;
        case CANCEL:
            layer_off(_NUM);
            layer_off(_LMOD);
            layer_off(_RMOD);
            disable_gaming();
            return false;
        case TO_GAME:
            if (record->event.pressed) {
                enable_gaming();
            }
            return false;
        case KC_TAB:
            if (record->event.pressed) {
                switch (get_highest_layer(layer_state)) {
                    case _LMOD:
                    case _RMOD:
                        tap_code16(C(S(KC_TAB)));
                        break;
                    case _NAV:
                        tap_code16(C(KC_TAB));
                        break;
                    default:
                        tap_code16(KC_TAB);
                }
            }
            return false;
        case KC_ENT:
            if (record->event.pressed) {
                switch (get_highest_layer(layer_state)) {
                    case _NUM:
                        tap_code(KC_PENT);
                        break;
                    default:
                        tap_code(KC_ENT);
                }
            }
            return false;
        case SE_AMPR:
        case SE_PIPE:
        case SE_PLUS:
        case SE_ASTR:
        case SE_MINS:
        case SE_UNDS:
        case SE_LABK:
        case SE_RABK:
        case SE_HASH:
        case SE_BSLS:
        case SE_SLSH:
        case SE_DQUO:
        case SE_DOT:
        case SE_EQL:
        case SE_EXLM:
        case SE_PERC:
        case GRV:
        case SE_LPRN:
        case SE_LCBR:
        case SE_LBRC:
            process_tap_hold(keycode, record);
            return false;
    }

    return true;
}

void tap_hold_send_hold(uint16_t keycode) {
    switch (keycode) {
        case SE_AMPR:
        case SE_PIPE:
        case SE_PLUS:
        case SE_ASTR:
        case SE_MINS:
        case SE_UNDS:
        case SE_LABK:
        case SE_RABK:
        case SE_HASH:
        case SE_BSLS:
        case SE_SLSH:
            double_tap(keycode);
            return;
        case SE_DQUO:
        case SE_DOT:
        case SE_EQL:
            triple_tap(keycode);
            return;
        case SE_EXLM:
            send_string("!=");
            return;
        case SE_PERC:
            send_string("%{}");
            return;
        case GRV:
            tap_undead_key(true, SE_GRV);
            tap_undead_key(true, SE_GRV);
            tap_undead_key(true, SE_GRV);
            return;
        case SE_LPRN:
            double_parens_left(keycode, SE_RPRN);
            return;
        case SE_LCBR:
            double_parens_left(keycode, SE_RCBR);
            return;
        case SE_LBRC:
            double_parens_left(keycode, SE_RBRC);
            return;
    }
}
void tap_hold_send_tap(uint16_t keycode) {
    switch (keycode) {
        case GRV:
            tap_undead_key(true, SE_GRV);
            return;
        default:
            tap_code16(keycode);
    }
}

void matrix_scan_user(void) {
    tap_hold_matrix_scan();
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

void oled_task_user(void) {
    render_status();
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        action_left_encoder(clockwise);
#    ifdef OLED_DRIVER_ENABLE
        oled_on();
#    endif
    } else if (index == 1) {
        action_right_encoder(clockwise);
#    ifdef OLED_DRIVER_ENABLE
        oled_on();
#    endif
    }
}
#endif

#ifdef TAPPING_TERM_PER_KEY

#define THUMB_TERM 20
#define INDEX_TERM -20
#define MIDDLE_TERM 0
#define RING_TERM 80
#define PINKY_TERM 180

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT_SPC:
            return TAPPING_TERM + THUMB_TERM;
        case DN_CTRL:
            return TAPPING_TERM + MIDDLE_TERM;
        default:
            return TAPPING_TERM;
    }
}
#endif
