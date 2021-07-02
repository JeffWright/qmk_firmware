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
//#include "tap_hold.h"
//#include "repeat.h"

//#include "keymap_swedish.h"
//#include "sendstring_swedish.h"
#include "g/keymap_combo.h"

#ifdef ENCODER_ENABLE
#    include "encoder_utils.h"
#endif

#ifdef OLED_DRIVER_ENABLE
#    include "oled_utils.h"
#endif

#define ALT_TAB_TIMEOUT 1000


bool is_alt_tab_active = false;
bool is_alt_tab_held = false;
uint16_t alt_tab_timer = 0;
uint16_t alt_with_tab(void);
void press_alt_tab(void);
void release_alt_tab(void);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
    * Base Layer: Modified RSTHD
    */
    [_BASE] = LAYOUT(
      xxxxxxx, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       xxxxxxx,
      KC_ESC,  HRM_A,   HRM_S,   HRM_D,   HRM_F,   KC_G,                                        KC_H,    HRM_J,   HRM_K,   HRM_L,   HRM_SCLN,   KC_QUOTE,
      KC_LSHIFT,KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_N,    KC_M,    KC_COMMA,KC_DOT,  KC_SLASH,   xxxxxxx,
                                 NUMWORD, ALTTAB, CMD_OR_CTRL, MT_SPC,  TAB_NAV,  ENTR_NUM, BSP_SYM,   DEL_FUN,  KC_HYPR, KC_F2
    ),
    [_SYM] = LAYOUT(
      xxxxxxx, KC_GRAVE,KC_MINUS,KC_LCBR, KC_RCBR,   KC_CIRC,                                     KC_PERCENT,xxxxxxx,xxxxxxx,xxxxxxx, KC_HASH,   xxxxxxx,
      KC_CIRC, KC_AT,   KC_PLUS, KC_LPRN, KC_RPRN, KC_EQUAL,                                    xxxxxxx, HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK,   KC_DOLLAR,
      _______, KC_AMPR, KC_ASTR, KC_TILDE,KC_DOLLAR,KC_PIPE,xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, KC_BSLASH, xxxxxxx,
                                 _______, xxxxxxx, _______, KC_SPC,  xxxxxxx, xxxxxxx, xxxxxxx, _______, xxxxxxx, _______
    ),
    [_NAV] = LAYOUT(
      xxxxxxx,         DYN_MACRO_PLAY1, LGUI(KC_LBRC),xxxxxxx,LGUI(KC_RBRC), xxxxxxx,                           xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, 
      KC_ESC,          HRM_PNK,         HRM_RNG, HRM_MID, HRM_IDX, xxxxxxx,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, xxxxxxx, xxxxxxx, 
      xxxxxxx,         xxxxxxx,         xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  xxxxxxx, xxxxxxx,
                                        _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    [_NUM] = LAYOUT(
      xxxxxxx, KC_HASH, KC_7,    KC_8,    KC_9,    KC_PERCENT,                                  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
      KC_ESC,  xxxxxxx, KC_4,    KC_5,    KC_6,    KC_EQUAL,                                    xxxxxxx, HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK, xxxxxxx, 
      xxxxxxx, xxxxxxx, KC_1,    KC_2,    KC_3,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, xxxxxxx,
                                 _______, CANCEL,  _______, KC_0,    KC_MINUS,_______, _______, _______, xxxxxxx, _______
    ),
    [_FUN] = LAYOUT(
      xxxxxxx, xxxxxxx, KC_F7,   KC_F8,   KC_F9,    KC_F10,                                      KC_F13,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, RESET,
      xxxxxxx, xxxxxxx, KC_F4,   KC_F5,   KC_F6,    KC_F11,                                      KC_F14,  HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK, xxxxxxx, 
      xxxxxxx, xxxxxxx, KC_F1,   KC_F2,   KC_F3,    KC_F12,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_F15,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, _______, _______, _______, _______, xxxxxxx, xxxxxxx
    ),
    // Important that the symbols on the base layer have the same positions as these symbols
    /*
    [_LMOD] = LAYOUT(
      xxxxxxx, OPT,     C(KC_A), C(KC_W), xxxxxxx, xxxxxxx,                                     xxxxxxx, KC_HASH, KC_AT,   KC_DOT,  REPEAT,  xxxxxxx,
      xxxxxxx, OS_ALT,  OS_SHFT, OS_CTRL, OS_GUI,  xxxxxxx,                                     GRV,     KC_QUES, KC_LBRC, KC_RBRC, xxxxxxx, xxxxxxx,
      xxxxxxx, C(KC_Z), C(KC_X), xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_SLSH, KC_AMPR, KC_LPRN, KC_RPRN, KC_UNDS, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, CLEAR,   FUN,     xxxxxxx, _______
    ),
    [_RMOD] = LAYOUT(
      xxxxxxx, TILD,    KC_PLUS, KC_ASTR, KC_PERC, xxxxxxx,                                     xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, SPEC,    xxxxxxx,
      xxxxxxx, KC_PIPE, KC_LCBR, KC_RCBR, KC_MINS, KC_BSLS,                                     xxxxxxx, OS_GUI,  OS_CTRL, OS_SHFT, OS_ALT,  xxxxxxx,
      xxxxxxx, KC_COMM, KC_LABK, KC_RABK, KC_EXLM, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, MY_RALT, xxxxxxx,
                                 _______, xxxxxxx, FUN,     _______, xxxxxxx, xxxxxxx, CLEAR,  _______, xxxxxxx, _______
    ),
    [_WNAV] = LAYOUT(
      xxxxxxx, xxxxxxx, G(KC_C), G(KC_K), xxxxxxx, G(KC_J),                                     xxxxxxx, G(KC_W), G(KC_E), G(KC_R), xxxxxxx, xxxxxxx,
      xxxxxxx, G(KC_6), G(KC_4), G(KC_0), G(KC_2), xxxxxxx,                                     xxxxxxx, G(KC_3), G(KC_1), G(KC_5), G(KC_7), xxxxxxx,
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, G(KC_8), xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, G(KC_9), G(KC_H), G(KC_L), xxxxxxx, xxxxxxx,
                                 _______, xxxxxxx, _______, G(KC_SPC),xxxxxxx,xxxxxxx, CLEAR,   _______, xxxxxxx, _______
    ),
    [_SPEC] = LAYOUT(
      xxxxxxx, KC_TILD, _______, _______, _______, _______,                                     _______, _______, _______, xxxxxxx, _______, xxxxxxx,
      xxxxxxx, _______, _______, _______, _______, xxxxxxx,                                     KC_GRV,  SYM_LFT, SYM_DWN, SYM_UP,  SYM_RHT, xxxxxxx,
      xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    */
    [_OPT] = LAYOUT(
      DYN_REC_STOP, DYN_REC_START1, DYN_REC_START2, _______, _______, _______,                                     _______, TG_NIX,  _______, _______, _______, RESET,  
      xxxxxxx,      _______,        _______,        _______, _______, TO_GAME,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx,      _______,        _______,        _______, _______, _______, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx,
                                 _______, xxxxxxx, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
    ),
    [_GAME] = LAYOUT(
      KC_T,    KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,                                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    xxxxxxx,
      KC_G,    KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,                                        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, xxxxxxx,
      KC_B,    KC_ENT,  KC_Z,    KC_X,    KC_C,    KC_V,    xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, KC_N,    KC_M,    KC_COMMA,KC_DOT,  KC_SLASH, xxxxxxx,
                                 _______, KC_LALT, KC_LCTL,   KC_SPC,  GAME2, CANCEL,  CANCEL,  _______, xxxxxxx, _______
    ),
    [_GAME2] = LAYOUT(
      xxxxxxx, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______, _______, _______, xxxxxxx,
                                 _______, _______, _______, _______, xxxxxxx, xxxxxxx, _______, _______, xxxxxxx, _______
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

// Keyboard utils

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

void tap16_repeatable(uint16_t keycode) {
    tap_code16(keycode);
    //register_key_to_repeat(keycode);
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

void enable_gaming(void) {
    /* autoshift_disable(); */
    layer_on(_GAME);
}
void disable_gaming(void) {
    /* autoshift_enable(); */
    layer_off(_GAME);
    layer_off(_GAME2);
}

void tap_space_shift(uint16_t key, bool key_down) {
    if (key_down) {
        tap_code16(key);
        tap_code(KC_SPC);
        //set_oneshot_mods(MOD_BIT(KC_LSFT));
    }
}

void double_tap(uint16_t keycode) {
    tap_code16(keycode);
    tap_code16(keycode);
}

// Combos

uint16_t get_combo_term(uint16_t index, combo_t *combo) {

            return COMBO_TERM + 25;
	    /*
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
            return COMBO_TERM + 25;
    }
   */
}

bool get_combo_must_tap(uint16_t index, combo_t *combo) {
    //switch (index) {
        //case del:
        //case backsp:
        //case q_comb:
        //case z_comb:
        //case num:
		/*
        case sp_ampr:
        case sp_pipe:
        case sp_plus:
        case sp_astr:
        case sp_mins:
        case sp_perc:
        case sp_grv:
        case sp_labk:
        case sp_rabk:
        case sp_lcbr:
        case sp_bsls:
        case sp_hash:
        case rev_rep:
        case lprn_arng:
        case rprn_adia:
        case unds_odia:
        case eql:
	*/
            //return false;
        //default:
            return true;
    //}
}

// Tapping terms

#ifdef TAPPING_TERM_PER_KEY

#define MIDDLE_TERM_EXTRA 0
#define RING_TERM_EXTRA 50
#define PINKY_TERM_EXTRA 150

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_SCLN:
            return TAPPING_TERM + PINKY_TERM_EXTRA;
        case HRM_S:
        case HRM_L:
            return TAPPING_TERM + RING_TERM_EXTRA;
        case HRM_D:
        case HRM_K:
            return TAPPING_TERM + MIDDLE_TERM_EXTRA;
        default:
            return TAPPING_TERM;
    }
}
#endif

// Case modes

bool terminate_case_modes(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
        // Keycodes to ignore (don't disable caps word)
        
        case CAPSWORD:

        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        case KC_MINS:
        case KC_UNDS:
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

/* overrideable function to determine whether to use the default separator on
 * first keypress when waiting for the separator. */
bool use_default_xcase_separator(uint16_t keycode, const keyrecord_t *record) {
    switch (keycode) {
         case KC_A ... KC_Z:
         case KC_1 ... KC_0:
             return true;
    }
    return false;
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

// One-shot mods
/*

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

void process_oneshot_pre(uint16_t keycode, keyrecord_t *record) {
    update_oneshot_pre(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot_pre(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot_pre(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot_pre(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );
}

void process_oneshot_post(uint16_t keycode, keyrecord_t *record) {
    update_oneshot_post(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot_post(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot_post(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot_post(
        &os_gui_state, KC_LGUI, OS_GUI,
        keycode, record
    );
}
*/

// Tap hold

bool tap_hold(uint16_t keycode) {
    switch (keycode) {
        case KC_DQUO:
        case KC_AMPR:
        case KC_PIPE:
        case KC_PLUS:
        case KC_ASTR:
        case KC_MINS:
        case KC_UNDS:
        case KC_LABK:
        case KC_RABK:
        case KC_HASH:
        case KC_BSLS:
        case KC_SLSH:
        case KC_DOT:
        case KC_EQL:
        case KC_EXLM:
        case KC_PERC:
        case GRV:
        case KC_LPRN:
        case KC_LCBR:
        case KC_LBRC:
        case KC_0:
        case G(KC_0):
        case G(KC_1):
        case G(KC_2):
        case G(KC_3):
        case G(KC_4):
        case G(KC_5):
        case G(KC_6):
        case G(KC_7):
        case G(KC_8):
        case G(KC_9):
        case G(KC_K):
        case G(KC_J):
        case G(KC_W):
        case G(KC_E):
        case G(KC_R):
        case G(KC_C):
        case KC_A ... KC_Z:
        //case KC_ARNG:
        //case KC_ADIA:
        //case KC_ODIA:
        //case LPRN_ARNG:
        //case RPRN_ADIA:
        //case UNDS_ODIA:
            return true;
        default:
            return false;
    }
}

void tap_hold_send_hold(uint16_t keycode) {
    switch (keycode) {
        case KC_AMPR:
        case KC_PIPE:
        case KC_PLUS:
        case KC_ASTR:
        case KC_MINS:
        case KC_UNDS:
        case KC_LABK:
        case KC_RABK:
        case KC_HASH:
        case KC_BSLS:
        case KC_SLSH:
        case KC_EQL:
            double_tap(keycode);
            return;
        case KC_DQUO:
        case KC_DOT:
        case KC_0:
            triple_tap(keycode);
            return;
        case KC_PERC:
            send_string("%{}");
            return;
        case GRV:
            tap_undead_key(true, KC_GRV);
            tap_undead_key(true, KC_GRV);
            tap_undead_key(true, KC_GRV);
            return;
        case KC_LPRN:
            double_parens_left(keycode, KC_RPRN);
            return;
        case KC_LCBR:
            double_parens_left(keycode, KC_RCBR);
            return;
        case KC_LBRC:
            double_parens_left(keycode, KC_RBRC);
            return;
	    /*
        case LPRN_ARNG:
            if (IS_LAYER_ON(_SWE)) {
                tap16_repeatable(KC_LPRN);
            } else {
                tap16_repeatable(S(KC_ARNG));
            }
            return;
        case RPRN_ADIA:
            if (IS_LAYER_ON(_SWE)) {
                tap16_repeatable(KC_RPRN);
            } else {
                tap16_repeatable(S(KC_ADIA));
            }
            return;
        case UNDS_ODIA:
            if (IS_LAYER_ON(_SWE)) {
                tap16_repeatable(KC_UNDS);
            } else {
                tap16_repeatable(S(KC_ODIA));
            }
            return;
	    */
        default:
            tap16_repeatable(S(keycode));
    }
}
void tap_hold_send_tap(uint16_t keycode) {
    switch (keycode) {
        case GRV:
            //register_key_to_repeat(keycode);
            tap_undead_key(true, KC_GRV);
            return;
	    /*
        case LPRN_ARNG:
            if (IS_LAYER_ON(_SWE)) {
                tap16_repeatable(KC_LPRN);
            } else {
                tap16_repeatable(KC_ARNG);
            }
            return;
        case RPRN_ADIA:
            if (IS_LAYER_ON(_SWE)) {
                tap16_repeatable(KC_RPRN);
            } else {
                tap16_repeatable(KC_ADIA);
            }
            return;
        case UNDS_ODIA:
            if (IS_LAYER_ON(_SWE)) {
                tap16_repeatable(KC_UNDS);
            } else {
                tap16_repeatable(KC_ODIA);
            }
            return;
	    */
        default:
            tap16_repeatable(keycode);
    }
}

uint16_t tap_hold_timeout(uint16_t keycode) {
    switch (keycode) {
        // Thumb
        case KC_E:
            return 120;
        // Pinky
        case KC_R:
        case KC_COMM:
        case KC_O:
        case KC_UNDS:
        case UNDS_ODIA:
            return 135;
        // Ring
        case KC_J:
        case KC_C:
        case KC_S:
        case KC_V:
        case KC_A:
        case KC_DOT:
        case KC_RPRN:
        case RPRN_ADIA:
            return 105;
        // Middle
        case KC_Y:
        case KC_T:
        case KC_G:
        case KC_U:
        case KC_I:
        case KC_LPRN:
        case LPRN_ARNG:
            return 100;
        // Index
        default:
            return 100;
    }
}

bool _process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_num_word(keycode, record)) {
        return false;
    }
    if (!process_case_modes(keycode, record)) {
        return false;
    }
    //if (!process_tap_hold(keycode, record)) {
        //return false;
    //}

    switch (keycode) {
	    case CMD_OR_CTRL:
		    if(in_linux()) {
			    // pretend it was KC_LCTRL
                if(record->event.pressed) {
                    register_code(KC_LCTRL);
                } else {
                    unregister_code(KC_LCTRL);
                }
		    } else {
			    // pretend it was KC_LGUI
                if(record->event.pressed) {
                    register_code(KC_LGUI);
                } else {
                    unregister_code(KC_LGUI);
                }
		    }
		    return false;
        //case KC_ESC:
            //return process_escape(record->event.pressed);
        //case KC_CAPS:
            //return process_caps(record->event.pressed);
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
            //clear_oneshot_mods();
            //if (get_oneshot_layer() != 0) {
                //clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
            //}
            layer_move(_BASE);
            return false;
        case TILD:
            //register_key_to_repeat(TILD);
            return tap_undead_key(record->event.pressed, KC_TILD);
        //case CIRC:
            //switch (get_highest_layer(layer_state)) {
                //case _SPEC:
                    //tap_code16(KC_CIRC);
                    //return false;
                //default:
                    //register_key_to_repeat(CIRC);
                    //return tap_undead_key(record->event.pressed, KC_CIRC);
            //}
        case TO_NUM:
            layer_on(_NUM);
            return false;
        case NUMWORD:
            process_num_word_activation(record);
            return false;
        case CAPSWORD:
            if (record->event.pressed) {
                toggle_caps_word();
//                if(caps_word_enabled()) {
 //                   disable_caps_word();
  //                  return false;
   //             } else {
    //                enable_caps_word();
     //               return false;
      //          }
                return false;
            }
        case XCASE:
            if (record->event.pressed) {
                if(get_xcase_state() == XCASE_OFF) {
                    enable_xcase_with(KC_UNDERSCORE);
                    enable_caps_word();
                    //enable_xcase();
                } else {
                    disable_xcase();
                }
            }
            return false;
        case ALTTAB:
            if(record->event.pressed) {
                press_alt_tab();
                return false;
            } else {
                release_alt_tab();
                return false;
            }
        case SAVE_VIM:
            if (record->event.pressed) {
                tap_escape();
                tap_code16(KC_COLN);
                tap_code(KC_W);
                tap_code(KC_ENT);
            }
            return false;
        //case CLOKC_VIM:
            //if (record->event.pressed) {
                //tap_escape();
                //tap_code16(KC_COLN);
                //tap_code(KC_Q);
                //tap_code(KC_ENT);
            //}
            //return false;
        case VIM_SP:
            if (record->event.pressed) {
                tap_code16(C(KC_W));
                tap_code(KC_S);
            }
            return false;
        case VIM_VS:
            if (record->event.pressed) {
                tap_code16(C(KC_W));
                tap_code(KC_V);
            }
            return false;
        case NUM_G:
            if (record->event.pressed) {
                tap_code16(S(KC_G));
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
                        tap16_repeatable(C(S(KC_TAB)));
                        break;
                    case _NAV:
                        tap16_repeatable(C(KC_TAB));
                        break;
                    default:
                        tap16_repeatable(KC_TAB);
                }
            }
            return false;
        case KC_ENT:
            if (record->event.pressed) {
                switch (get_highest_layer(layer_state)) {
                    case _NUM:
                        tap16_repeatable(KC_PENT);
                        break;
                    case _WNAV:
                        tap16_repeatable(G(KC_ENT));
                        break;
                    default:
                        tap16_repeatable(KC_ENT);
                }
            }
            return false;
        //case REPEAT:
            //update_repeat_key(record);
            //return false;
        //case REV_REP:
            //update_reverse_repeat_key(record);
            //return false;
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    //process_oneshot_pre(keycode, record);

    bool res = _process_record_user(keycode, record);

    // If `false` was returned, then we did something special and should register that manually.
    // Otherwise register it here by default.
    // Skip Space to not interfere with NAV toggling.
    //if (res && record->event.pressed && keycode != MT_SPC) {
        //register_key_to_repeat(keycode);
    //}

    //process_oneshot_post(keycode, record);

    return res;
}

uint16_t alt_with_tab() {
    if(in_linux()) {
        return KC_LALT;
    } else {
        return KC_LGUI;
    }
}

void press_alt_tab() {
    if (!is_alt_tab_active) {
        is_alt_tab_active = true;
        register_code(alt_with_tab());
    }
    is_alt_tab_held = true;
    alt_tab_timer = timer_read();
    tap_code16(KC_TAB);
}
void release_alt_tab() {
    is_alt_tab_held = false;
}


void matrix_scan_user(void) {
   // tap_hold_matrix_scan();

  if (is_alt_tab_active && !is_alt_tab_held) {
    if (timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
      unregister_code(alt_with_tab());
      is_alt_tab_active = false;
    }
  }
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
    /* for alt-tab:
	if (clockwise) {
	  if (!is_alt_tab_active) {
	    is_alt_tab_active = true;
	    register_code(alt_with_tab());
	  }
	  alt_tab_timer = timer_read();
	  tap_code16(KC_TAB);
	} else {
	  if (!is_alt_tab_active) {
	    is_alt_tab_active = true;
	    register_code(alt_with_tab());
	  }
	  alt_tab_timer = timer_read();
	  tap_code16(S(KC_TAB));
	}
    */
    if (clockwise) {
	  tap_code16(KC_MS_WH_DOWN);
	  tap_code16(KC_MS_WH_DOWN);
	  tap_code16(KC_MS_WH_DOWN);
    } else {
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
    }
}
#endif

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_S:
        case HRM_L:
            return false;
        default:
            return true;
    }
}

