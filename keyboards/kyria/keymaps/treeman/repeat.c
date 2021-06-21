#include "repeat.h"

uint16_t last_keycode = KC_NO;

void register_key_to_repeat(uint16_t keycode) {
    // Get the base keycode of a mod or layer tap key
    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            keycode = keycode & 0xFF;
            break;
    }

    // Merge current mod state with keycode, for easy comparison when
    // we want to do special key reverse repeats.
    uint8_t mods = get_mods() | get_oneshot_mods();
    if (mods & MOD_MASK_CTRL) keycode |= QK_LCTL;
    if (mods & MOD_MASK_SHIFT) keycode |= QK_LSFT;
    if (mods & MOD_MASK_GUI) keycode |= QK_LGUI;
    if (mods & MOD_BIT(KC_LALT)) keycode |= QK_LALT;
    if (mods & MOD_BIT(KC_RALT)) keycode |= QK_RALT;

    last_keycode = keycode;
}

void update_key(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        register_code16(keycode);
    } else {
        unregister_code16(keycode);
    }
}

void update_repeat_key(keyrecord_t *record) {
    update_key(last_keycode, record);
}

void update_reverse_repeat_key(keyrecord_t *record) {
    // Do the "reverse" of the last pressed key, that we use to repeat
    switch (last_keycode) {
        case C(KC_TAB):
            update_key(C(S(KC_TAB)), record);
            break;
        case C(S(KC_TAB)):
            update_key(S(KC_TAB), record);
            break;
        case C(KC_N):
            update_key(C(KC_P), record);
            break;
        case C(KC_P):
            update_key(C(KC_N), record);
            break;
        case C(KC_F):
            update_key(C(KC_B), record);
            break;
        case C(KC_B):
            update_key(C(KC_F), record);
            break;
        case C(KC_U):
            update_key(C(KC_D), record);
            break;
        case C(KC_D):
            update_key(C(KC_U), record);
            break;
        case S(KC_W):
            update_key(S(KC_B), record);
            break;
        case S(KC_B):
            update_key(S(KC_W), record);
            break;
        default:
            // Might make more sense to not do anything here
            // instead of doing something random.
            /* update_key(last_keycode, record); */
            break;
    }
}

