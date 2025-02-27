/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>
   Copyright 2022 Andreas

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "rgb_matrix_map.h"

enum my_keycodes {
  KC_GMTOGG = SAFE_RANGE, // game mode toggle
  KC_LSFTCAPS             // left shift in game mode instead of caps lock
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     /        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backspace. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR,          KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_DEL,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,                   KC_PGUP,
        KC_LSFTCAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,           KC_PGDN,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_END,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        KC_SLEP, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, _______, KC_PAUS,          _______,
        _______, RGB_TOG, RGB_M_P, RGB_M_B, RGB_M_R, RGB_M_SW, _______, _______, _______, _______, _______, _______, _______, RESET,            KC_INS,
        _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
        _______, RGB_HUD, RGB_VAD, RGB_HUI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_MOD, KC_HOME,
        _______, KC_GMTOGG, _______,                            _______,                            _______, _______, _______, RGB_SPD, RGB_RMOD, RGB_SPI
    ),


};
// clang-format on

#ifdef ENCODER_ENABLE
  bool encoder_update_user(uint8_t index, bool clockwise) 
  {
    if (clockwise) 
    {
      tap_code(KC_VOLU);
    } 
    else 
    {
      tap_code(KC_VOLD);
    }

    return true;
  }
#endif

#ifdef RGB_MATRIX_ENABLE
  void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) 
  {
    // Game mode enabled
    if (keymap_config.no_gui) 
    {
      rgb_matrix_set_color(LED_LWIN, RGB_RED);  // light up left windows key

      // light up side leds
      for (uint8_t i = 0; i < sizeof(LED_SIDE_LEFT) / sizeof(LED_SIDE_LEFT[0]); i++) 
      {
        rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_RED);
        rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_RED);
      }
    }

    // Caps lock enabled
    static uint32_t cycle_led_timer = 0;
    static bool cycle_led_active = false;

    if (host_keyboard_led_state().caps_lock)
    {
      if (timer_elapsed32(cycle_led_timer) > 500)
      {
          cycle_led_active = !cycle_led_active;
          cycle_led_timer = timer_read32();
      }

      if (cycle_led_active)
      {
        rgb_matrix_set_color(LED_CAPS, RGB_RED);
      }
      else
      {
        rgb_matrix_set_color(LED_CAPS, RGB_OFF);
      }

      for (uint8_t i = 0; i < sizeof(LED_SIDE_LEFT) / sizeof(LED_SIDE_LEFT[0]); i++) 
      {
        if (cycle_led_active)
        {
          rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_RED);
          rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_RED);
        }
        else
        {
          rgb_matrix_set_color(LED_SIDE_LEFT[i], RGB_OFF);
          rgb_matrix_set_color(LED_SIDE_RIGHT[i], RGB_OFF);
        }
      }
    }
  }
#endif

// if windows key is disabled (game mode) shift otherwise normal caps lock
void kc_LSFTCAP_register(void) {
    if (keymap_config.no_gui && !host_keyboard_led_state().caps_lock) {
        register_code16(KC_LSFT);
    } else {
        register_code(KC_CAPS);
    }
}

void kc_LSFTCAP_unregister(void) {
    if (keymap_config.no_gui) {
        unregister_code16(KC_LSFT);
    } else {
        unregister_code(KC_CAPS);
    }
}

// toggle game mode. Deactivate caps lock if activated
void kc_GMTOGG_register(void) {
  keymap_config.no_gui = !keymap_config.no_gui; // toggle game mode
  if (host_keyboard_led_state().caps_lock) register_code(KC_CAPS);
}

void kc_GMTOGG_unregister(void) {
  unregister_code16(KC_GMTOGG);
  if (host_keyboard_led_state().caps_lock) unregister_code(KC_CAPS);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) 
{
  switch (keycode) 
  {
    case KC_GMTOGG:
      if (record->event.pressed) 
      {
        kc_GMTOGG_register();
      } 
      else 
      {
        kc_GMTOGG_unregister();
      }

      return false; // skip all further processing of this key

    case KC_LSFTCAPS:
      if (record->event.pressed) 
      {
        kc_LSFTCAP_register();
      } 
      else 
      {
        kc_LSFTCAP_unregister();
      }

      return false; // skip all further processing of this key

    default:
      return true; // process all other keycodes normally
  }
}