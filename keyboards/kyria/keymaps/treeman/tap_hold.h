#pragma once

#include QMK_KEYBOARD_H

// Process tap hold, place in process_record_user(), when any key we want a hold action is pressed.
void process_tap_hold(uint16_t keycode, const keyrecord_t *record);

// Matrix scan, place in matrix_scan_user()
void tap_hold_matrix_scan(void);
// User can override this to do whatever tap action they want.
void tap_hold_send_tap(uint16_t keycode);
// User must define this to do whatever hold action they want.
void tap_hold_send_hold(uint16_t keycode);

void tap_hold_layer(uint16_t tap, uint8_t layer, uint16_t *timer, const keyrecord_t *record);
