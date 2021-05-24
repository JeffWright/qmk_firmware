#pragma once

#include QMK_KEYBOARD_H

/*
// Check whether symword is on
bool sym_word_enabled(void);
// Disable symword
void disable_sym_word(void);
// Process symword activation, to be placed in process user as a keycode
void process_sym_word_activation(const keyrecord_t *record);
// Process symword, to be placed in process user
bool process_sym_word(uint16_t keycode, const keyrecord_t *record);
*/

// Check whether numword is on
bool num_word_enabled(void);
// Enable numword
void enable_num_word(void);
// Disable numword
void disable_num_word(void);
// Process numword activation, to be placed in process user as a keycode
void process_num_word_activation(const keyrecord_t *record);
// Process numword, to be placed in process user
bool process_num_word(uint16_t keycode, const keyrecord_t *record);

