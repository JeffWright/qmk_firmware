#include "tap_hold.h"

static bool in_progress = false;
static uint16_t timer = 0;
static uint16_t lastkey = KC_NO;

void process_tap_hold(uint16_t keycode, const keyrecord_t *record) {
    if (record->event.pressed) {
        in_progress = true;
        timer = timer_read();
        lastkey = keycode;
    } else {
        if (in_progress && timer_elapsed(timer) < AUTO_SHIFT_TIMEOUT) {
            in_progress = false;
            tap_hold_send_tap(keycode);
        }
    }
}

void tap_hold_matrix_scan() {
    if (in_progress && timer_elapsed(timer) >= AUTO_SHIFT_TIMEOUT) {
        in_progress = false;
        tap_hold_send_hold(lastkey);
    }
}

__attribute__ ((weak))
void tap_hold_send_tap(uint16_t keycode) {
    tap_code16(keycode);
}

// FIXME try this out
// Also need to check cancelations for *word impls
void tap_hold_layer(uint16_t tap, uint8_t layer, uint16_t *timer, const keyrecord_t *record) {
    if (record->event.pressed) {
        layer_on(layer);
        *timer = timer_read();
    } else {
        if (timer_elapsed(*timer) < AUTO_SHIFT_TIMEOUT) {
            tap_code16(tap);
        } else {
            layer_off(layer);
        }
    }
}
