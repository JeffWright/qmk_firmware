
// Keyboard utils

static bool linux_mode = true;
bool in_linux(void) {
    return linux_mode;
}

void tg_nix(void) {
    linux_mode = !linux_mode;
#   ifdef OLED_DRIVER_ENABLE
        oled_on();
#   endif
}

//#ifdef OLED_DRIVER_ENABLE
////oled_rotation_t oled_init_user(oled_rotation_t rotation) {
   //// return OLED_ROTATION_180;
////}

//void oled_task_user(void) {
   //// render_status();
//}
//#endif

// Combos

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    return COMBO_TERM + 25;
}

bool get_combo_must_tap(uint16_t index, combo_t *combo) {
    return true;
}

// Tapping terms

#ifdef TAPPING_TERM_PER_KEY

// Very slow tapping term for A/S/D and K/L/; because we never want them to trigger while typing
//#define SECONDARY_HRM_TERM_EXTRA 300

uint16_t secondary_term_extra = 300;


uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HRM_A:
        case HRM_SCLN:
        case HRM_S:
        case HRM_L:
        case HRM_D:
        case HRM_K:
            return TAPPING_TERM + secondary_term_extra;
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


bool _process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_case_modes(keycode, record)) {
        return false;
    }
    //if(!process_tap_hold(keycode, record)) {
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
        case TG_NIX:
            if (record->event.pressed) {
                tg_nix();
            }
            return false;
        case CAPSWORD:
            if (record->event.pressed) {
                toggle_caps_word();
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
        case ALTTAB_APP:
          if(in_linux()) {
            if(record->event.pressed) {
              register_code(KC_LCTRL);
              register_code(KC_TAB);
            } else {
              unregister_code(KC_TAB);
              unregister_code(KC_LCTRL);
            }
          } else {
            if(record->event.pressed) {
              register_code(KC_LGUI);
              register_code(KC_GRAVE);
            } else {
              unregister_code(KC_GRAVE);
              unregister_code(KC_LGUI);
            }
          }
          return false;
        case NAV_BACK:
          if(in_linux()) {
            if(record->event.pressed) {
              register_code(KC_LALT);
              register_code(KC_LEFT);
            } else {
              unregister_code(KC_LEFT);
              unregister_code(KC_LALT);
            }
          } else {
            if(record->event.pressed) {
              register_code(KC_LGUI);
              register_code(KC_LEFT);
            } else {
              unregister_code(KC_LEFT);
              unregister_code(KC_LGUI);
            }
          }
          return false;
        case NAV_FWD:
          if(in_linux()) {
            if(record->event.pressed) {
              register_code(KC_LALT);
              register_code(KC_RIGHT);
            } else {
              unregister_code(KC_RIGHT);
              unregister_code(KC_LALT);
            }
          } else {
            if(record->event.pressed) {
              register_code(KC_LGUI);
              register_code(KC_RIGHT);
            } else {
              unregister_code(KC_RIGHT);
              unregister_code(KC_LGUI);
            }
          }
          return false;
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

////////////////////////////////////////////////
   update_oneshot(
        &os_shft_state, KC_LSFT, OS_HYPR,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_HYPR,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_HYPR,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_HYPR,
        keycode, record
    );
////////////////////////////////////////////////

    if(linux_mode) {
        update_oneshot(
            &os_cmd_ctrl_state, KC_LCTL, OS_CMD_CTRL,
            keycode, record
        );
    } else {
        update_oneshot(
            &os_cmd_ctrl_state, KC_LCMD, OS_CMD_CTRL,
            keycode, record
        );
    }

    return _process_record_user(keycode, record);

    // If `false` was returned, then we did something special and should register that manually.
    // Otherwise register it here by default.
    // Skip Space to not interfere with NAV toggling.
    //if (res && record->event.pressed && keycode != MT_SPC) {
        //register_key_to_repeat(keycode);
    //}

    //process_oneshot_post(keycode, record);

//    return true;
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


LEADER_EXTERNS();

void matrix_scan_user(void) {

    if (is_alt_tab_active) {
        if (!is_alt_tab_held && timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
            unregister_code(alt_with_tab());
            is_alt_tab_active = false;
        }
    }

    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        SEQ_ONE_KEY(KC_E) {
            SEND_STRING("[");
        }
        SEQ_ONE_KEY(KC_R) {
            SEND_STRING("]");
        }
        SEQ_ONE_KEY(KC_D) {
            SEND_STRING("<");
        }
        SEQ_ONE_KEY(KC_F) {
            SEND_STRING(">");
        }

        // SEQ_ONE_KEY(KC_M) {
            // SEND_STRING("MET3-");
        // }
        SEQ_ONE_KEY(KC_J) {
            SEND_STRING("// TODO JTW: ");
        }
        SEQ_ONE_KEY(KC_A) {
            SEND_STRING("-> ");
        }
        // SEQ_TWO_KEYS(KC_S, KC_B) {
            // SEND_STRING("shouldBeEqualTo ");
            //SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
        // }
        // SEQ_TWO_KEYS(KC_A, KC_A) {
            // SEND_STRING("nmc12345");
            // SEND_STRING(SS_TAP(X_ENTER));

            // // SEND_STRING(KC_ENTER);
        // }
        // SEQ_TWO_KEYS(KC_A, KC_B) {
            // SEND_STRING("child1");
            // SEND_STRING(SS_TAP(X_ENTER));
        //}

        SEQ_TWO_KEYS(KC_T, KC_T) {

	    secondary_term_extra -= 10;
            SEND_STRING("tt=");

        }
        //SEQ_THREE_KEYS(KC_D, KC_D, KC_S) {
            //SEND_STRING("https://start.duckduckgo.com\n");
        //}
        //SEQ_TWO_KEYS(KC_A, KC_S) {
            //register_code(KC_LGUI);
            //register_code(KC_S);
            //unregister_code(KC_S);
            //unregister_code(KC_LGUI);
        //}
    }

}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_90;
}

//void oled_task_user(void) {
//    render_status();
//}
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
	  tap_code16(KC_MS_WH_DOWN);
	  tap_code16(KC_MS_WH_DOWN);
	  tap_code16(KC_MS_WH_DOWN);
    } else {
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
	  tap_code16(KC_MS_WH_UP);
    }
}
#endif

//bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
//    switch (keycode) {
//        case HRM_A:
//        case HRM_S:
//        case HRM_L:
//            return false;
//        default:
//            return true;
//    }
//}



// One-shot mods


bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case LA_NUM:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case LA_NUM:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
    case OS_HYPR:
    case OS_CMD_CTRL:
        return true;
    default:
        return false;
    }
}


bool tap_hold(uint16_t keycode) {
    switch (keycode) {
//    case LA_SYM:
    case KC_A ... KC_Z:
    case KC_SPC:
        return true;
    default:
        return false;
    }
}














#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master()) {
    return OLED_ROTATION_90;  // flips the display 180 degrees if offhand
  }
  return OLED_ROTATION_270;
}


//void oled_render_layer_state(void) {
//    oled_write_P(PSTR("Layer: "), false);
//
//    switch (get_highest_layer(layer_state)) {
//        case _BASE:
//            oled_write_ln_P(PSTR("Base"), false);
//            break;
//        case _SYM:
//            oled_write_ln_P(PSTR("Symbol"), false);
//            break;
//    }
//
//}


void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
    render_status();
//    if (is_keyboard_master()) {
//        render_master_status();
//    } else {
//        oled_render_logo();
//    }
}

#endif // OLED_ENABLE
