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

#define LAYOUT_common( \
    L00, L01, L02, L03, L04, L05,      R00, R01, R02, R03, R04, R05, \
    L10, L11, L12, L13, L14, L15,      R10, R11, R12, R13, R14, R15, \
    L20, L21, L22, L23, L24, L25,      R20, R21, R22, R23, R24, R25, \
                     L30, L31, L32,  R30, R31, R32 \
  ) \
  LAYOUT( \
    L00, L01, L02, L03, L04, L05,                                     R00, R01, R02, R03, R04, R05, \
    L10, L11, L12, L13, L14, L15,                                     R10, R11, R12, R13, R14, R15, \
    L20, L21, L22, L23, L24, L25, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, R20, R21, R22, R23, R24, R25, \
                   xxxxxxx, L30, L31, L32, xxxxxxx, xxxxxxx, R30, R31, R32, xxxxxxx \
    )

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_common(
      ALTTAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       KC_BSPACE,
      KC_ESC,   HRM_A,  HRM_S,   HRM_D,   HRM_F,   KC_G,                                     KC_H,    HRM_J,   HRM_K,   HRM_L,   HRM_SCLN,   KC_QUOTE,
      KC_LSHIFT,KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,                                     KC_N,    KC_M,    KC_COMMA,KC_DOT,  KC_SLASH,   DEL_FNKEYS,
                                        CMD_OR_CTRL, LA_SYM,  xxxxxxx,           KC_SPC,  LA_NUM,  LA_NAV
    ),
    [_SYM] = LAYOUT_common(
      ALTTAB_APP, KC_GRAVE,KC_MINUS,KC_LCBR, KC_RCBR, KC_CIRC,                                       KC_PERCENT, xxxxxxx,xxxxxxx,xxxxxxx, KC_HASH,     _______,
      KC_CIRC,    KC_AT,   KC_PLUS, KC_LPRN, KC_RPRN, KC_EQUAL,                                      xxxxxxx,   HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK,   KC_DOLLAR,
      _______,    KC_AMPR, KC_ASTR, KC_TILDE,KC_DOLLAR,KC_PIPE,                                      xxxxxxx,   xxxxxxx, _______, _______, KC_BSLASH, xxxxxxx,
                                       _______, xxxxxxx, xxxxxxx,                    _______, xxxxxxx,  xxxxxxx
    ),

    [_NAV] = LAYOUT_common(
      xxxxxxx,         DYN_MACRO_PLAY1, NAV_BACK,      xxxxxxx, NAV_FWD,       xxxxxxx,                           xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, TG_NIX,
      KC_ESC,          HRM_PNK,         HRM_RNG,       HRM_MID, HRM_IDX,       OS_HYPR,                           KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, xxxxxxx, RESET,
      xxxxxxx,         xxxxxxx,         xxxxxxx,       xxxxxxx, OS_CMD_CTRL,   xxxxxxx,                           KC_HOME, KC_PGDN, KC_PGUP, KC_END,  xxxxxxx, xxxxxxx,
                                                                          xxxxxxx, xxxxxxx,xxxxxxx,   _______, xxxxxxx,  xxxxxxx
    ),

    [_NUM] = LAYOUT_common(
      xxxxxxx, KC_HASH, KC_7,    KC_8,    KC_9,    KC_PERCENT,                                  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, _______,
      KC_ESC,  xxxxxxx, KC_4,    KC_5,    KC_6,    KC_EQUAL,                                    OS_HYPR, HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK, KC_DOLLAR,
      xxxxxxx, xxxxxxx, KC_1,    KC_2,    KC_3,    xxxxxxx,                                     xxxxxxx, OS_CMD_CTRL,_______,_______,_______, xxxxxxx,
                                                      _______, KC_0, xxxxxxx,     _______, xxxxxxx, xxxxxxx
    ),
    [_OPT] = LAYOUT_common(
      xxxxxxx, xxxxxxx, xxxxxxx, _______, _______, _______,                                     _______, TG_NIX,  _______, _______, _______, RESET,
      xxxxxxx,      _______,        _______,        _______, _______, xxxxxxx,                                     _______, _______, _______, _______, _______, xxxxxxx,
      xxxxxxx,      _______,        _______,        _______, _______, _______,                                     _______, _______, _______, _______, _______, xxxxxxx,
                                                                   xxxxxxx, xxxxxxx, xxxxxxx,  _______, xxxxxxx,  xxxxxxx
    ),
    [_FNKEYS] = LAYOUT_common(
      xxxxxxx, xxxxxxx, KC_F7,   KC_F8,   KC_F9,    KC_F10,                                      KC_F13,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, RESET,
      xxxxxxx, xxxxxxx, KC_F4,   KC_F5,   KC_F6,    KC_F11,                                      KC_F14,  HRM_IDX, HRM_MID, HRM_RNG, HRM_PNK, xxxxxxx,
      xxxxxxx, xxxxxxx, KC_F1,   KC_F2,   KC_F3,    KC_F12,                                      KC_F15,  xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx, xxxxxxx,
                                                 _______, xxxxxxx, xxxxxxx,  _______, xxxxxxx,  xxxxxxx
    )
};


// Keyboard utils
#include "JeffWright.c"
