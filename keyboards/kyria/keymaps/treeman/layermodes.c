#include "layermodes.h"
#include "keycodes.h"

/*
static uint16_t sym_word_timer;
static bool _sym_word_enabled = false;
bool sym_word_enabled(void) {
    return _sym_word_enabled;
}
void disable_sym_word(void) {
    _sym_word_enabled = false;
    layer_off(_SYM);
}
void process_sym_word_activation(const keyrecord_t *record) {
    if (record->event.pressed) {
        layer_on(_SYM);
        sym_word_timer = timer_read();
    } else {
        if (timer_elapsed(sym_word_timer) < TAPPING_TERM) {
            // Tapping enables SYMWORD
            _sym_word_enabled = true;
        } else {
            // Holding turns off SYM when released
            layer_off(_SYM);
        }
    }
}
bool process_sym_word(uint16_t keycode, const keyrecord_t *record) {
    if (!_sym_word_enabled) return true;

    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            if (record->tap.count == 0)
                return true;
            keycode = keycode & 0xFF;
    }
    switch (keycode) {
        case GRV:
        case SE_LCBR:
        case SE_RCBR:
        case SE_PERC:
        case SE_HASH:
        case TILD:
        case SE_PLUS:
        case SE_ASTR:
        case SE_MINS:
        case SE_EQL:
        case SE_PIPE:
        case SE_AT:
        case SE_QUES:
        case SE_RBRC:
        case SE_LBRC:
        case SE_AMPR:
        case SE_LABK:
        case SE_RABK:
        case SE_BSLS:
        case SE_EXLM:
        case SE_COMM:
        case SE_DOT:
        case SE_UNDS:
        case SE_RPRN:
        case SE_LPRN:
        case SE_SLSH:
        case CIRC:
        case SE_DLR:
        case KC_BSPC:
        case SE_SCLN:
        case SE_COLN:
        case xxxxxxx:
            // Don't disable for the above keycodes
            break;
        case CANCEL:
            if (record->event.pressed) {
                disable_sym_word();
            }
            return false;
        default:
            if (record->event.pressed) {
                disable_sym_word();
            }
    }
    return true;
}
*/


static uint16_t num_word_timer;
static bool _num_word_enabled = false;
bool num_word_enabled(void) {
    return _num_word_enabled;
}
void enable_num_word(void) {
    _num_word_enabled = true;
    layer_on(_NUM);
}
void disable_num_word(void) {
    _num_word_enabled = false;
    layer_off(_NUM);
}
void process_num_word_activation(const keyrecord_t *record) {
    if (record->event.pressed) {
        layer_on(_NUM);
        num_word_timer = timer_read();
    } else {
        if (timer_elapsed(num_word_timer) < TAPPING_TERM) {
            // Tapping enables NUMWORD
            _num_word_enabled = true;
        } else {
            // Holding turns off NUM when released
            layer_off(_NUM);
        }
    }
}
bool process_num_word(uint16_t keycode, const keyrecord_t *record) {
    if (!_num_word_enabled) return true;

    switch (keycode) {
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            if (record->tap.count == 0)
                return true;
            keycode = keycode & 0xFF;
    }
    switch (keycode) {
        case SE_1 ... SE_0:
        case MY_000:
        case SE_PERC:
        case SE_COMM:
        case SE_DOT:
        case SE_SLSH:
        case SE_MINS:
        case SE_ASTR:
        case SE_PLUS:
        case SE_EQL:
        case SE_UNDS:
        case KC_BSPC:
        case xxxxxxx:
            // Don't disable for above keycodes
            break;
        case CANCEL:
            if (record->event.pressed) {
                disable_num_word();
            }
            return false;
        default:
            if (record->event.pressed) {
                disable_num_word();
            }
    }
    return true;
}

