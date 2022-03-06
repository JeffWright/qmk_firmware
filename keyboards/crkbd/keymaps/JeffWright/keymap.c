#include QMK_KEYBOARD_H

#include "keycodes.h"
#include "status.h"
#include "oneshot.h"
#include "casemodes.h"
#include "quantum.h"
#include "g/keymap_combo.h"

//#ifdef OLED_DRIVER_ENABLE
#    include "oled_utils.h"
//#endif

#define ALT_TAB_TIMEOUT 1000

bool is_alt_tab_active = false;
bool is_alt_tab_held = false;
uint16_t alt_tab_timer = 0;
uint16_t alt_with_tab(void);
void press_alt_tab(void);
void release_alt_tab(void);

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;
oneshot_state os_hypr_state = os_up_unqueued;
oneshot_state os_cmd_ctrl_state = os_up_unqueued;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
      ALTTAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       KC_BSPACE,
      KC_ESC,   KC_A,   KC_S,    KC_D,    HRM_F,   KC_G,                                          KC_H,    HRM_J,   KC_K,    KC_L,    KC_SCLN,    KC_QUOTE,
      KC_LSHIFT,KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                                          KC_N,    KC_M,    KC_COMMA,KC_DOT,  KC_SLASH,   xxxxxxx,
                                          LA_NAV, CMD_OR_CTRL, LA_SYM,                         KC_SPC,   LA_NUM,  DEL_FNKEYS
    ),
    [_SYM] = LAYOUT(
      xxxxxxx, KC_GRAVE,KC_MINUS,KC_LCBR, KC_RCBR, KC_CIRC,                                     KC_PERCENT,xxxxxxx,xxxxxxx,xxxxxxx, KC_HASH,   xxxxxxx,
      KC_CIRC, KC_AT,   KC_PLUS, KC_LPRN, KC_RPRN, KC_EQUAL,                                    xxxxxxx, HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK,   KC_DOLLAR,
      _______, KC_AMPR, KC_ASTR, KC_TILDE,KC_DOLLAR,KC_PIPE,                                    xxxxxxx, xxxxxxx, _______, _______, KC_BSLASH, xxxxxxx,
                                          xxxxxxx, _______, xxxxxxx,                   KC_SPC, _______, xxxxxxx
    ),

    [_NAV] = LAYOUT(
      xxxxxxx,         DYN_MACRO_PLAY1, NAV_BACK,      xxxxxxx, NAV_FWD,       xxxxxxx,                           xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, TG_NIX,
      KC_ESC,          HRM_PNK,         HRM_RNG,       HRM_MID, HRM_IDX,       OS_HYPR,                           KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, xxxxxxx, xxxxxxx,
      xxxxxxx,         xxxxxxx,         xxxxxxx,       xxxxxxx, OS_CMD_CTRL,   xxxxxxx,                           KC_HOME, KC_PGDN, KC_PGUP, KC_END,  xxxxxxx, xxxxxxx,
                                                       xxxxxxx, _______, _______,                   _______, _______, xxxxxxx
    ),

    [_NUM] = LAYOUT(
      xxxxxxx, KC_HASH, KC_7,    KC_8,    KC_9,    KC_PERCENT,                                  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
      KC_ESC,  xxxxxxx, KC_4,    KC_5,    KC_6,    KC_EQUAL,                                    OS_HYPR, HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK, KC_DOLLAR,
      xxxxxxx, xxxxxxx, KC_1,    KC_2,    KC_3,    xxxxxxx,                                     xxxxxxx, OS_CMD_CTRL,_______,_______,_______, xxxxxxx,
                                          xxxxxxx,  _______, KC_0,                      _______, _______, xxxxxxx
    ),
    [_OPT] = LAYOUT(
      DYN_REC_STOP, DYN_REC_START1, DYN_REC_START2, _______, _______, _______,                                     _______, TG_NIX,  _______, _______, _______, RESET,
      xxxxxxx,      _______,        _______,        _______, _______, xxxxxxx,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx,      _______,        _______,        _______, _______, _______,                                     _______, _______, _______, _______, _______, xxxxxxx,
                                          xxxxxxx, _______, _______,                   _______, _______, xxxxxxx
    ),
    [_FNKEYS] = LAYOUT(
      xxxxxxx, xxxxxxx, KC_F7,   KC_F8,   KC_F9,    KC_F10,                                      KC_F13,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, RESET,
      xxxxxxx, xxxxxxx, KC_F4,   KC_F5,   KC_F6,    KC_F11,                                      KC_F14,  HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK, xxxxxxx,
      xxxxxxx, xxxxxxx, KC_F1,   KC_F2,   KC_F3,    KC_F12,                                      KC_F15,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
                                          xxxxxxx, _______, _______,                   _______, _______, xxxxxxx
    )
};

// Keyboard utils
#include "JeffWright.c"
