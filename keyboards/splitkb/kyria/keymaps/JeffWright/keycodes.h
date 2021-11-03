#pragma once 
#include QMK_KEYBOARD_H

//#include "keymap_swedish.h"
//#include "quantum.h"

enum layers {
    _BASE = 0,
    _SYM,
    _NUM,
    _NAV,
    _OPT,
    _FNKEYS
};

#define BSP_SYM LT(_SYM, KC_BSPACE)
#define TAB_NAV LT(_NAV, KC_TAB)
#define ENTR_NUM LT(_NUM, KC_ENTER)
#define DEL_FNKEYS LT(_FNKEYS, KC_DELETE)

#define HRM_PNK  KC_LGUI
#define HRM_A    LGUI_T(KC_A)
#define HRM_SCLN RGUI_T(KC_SCLN)

#define HRM_RNG  KC_LALT
#define HRM_S    LALT_T(KC_S)
#define HRM_L    RALT_T(KC_L)

#define HRM_MID  KC_LCTL
#define HRM_D    LCTL_T(KC_D)
#define HRM_K    RCTL_T(KC_K)

#define HRM_IDX  KC_LSHIFT
#define HRM_F LSFT_T(KC_F)
#define HRM_J RSFT_T(KC_J)

#define HRM_CLN RGUI_T(LSFT(KC_SCLN))


enum custom_keycodes {

    CMD_OR_CTRL = SAFE_RANGE,
    ALTTAB,

    CAPSWORD,
    XCASE,

    // Runtime options
    TG_NIX,
    TG_CAPS,
};

#define xxxxxxx KC_NO

