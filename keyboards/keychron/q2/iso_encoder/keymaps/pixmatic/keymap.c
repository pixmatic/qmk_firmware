/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "os_engine.h"
#include "pixmatic.h"

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Capa 0: Base
    [0] = LAYOUT_iso_68(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,  KC_EQL,   KC_BSPC,          KC_MUTE,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,                    KC_DEL,
        C_24,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_NUHS,  KC_ENT,           KC_HOME,
        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,            KC_RSFT, KC_UP,
        OS_L1,   OS_L2,   OS_L3,                            KC_SPC,                               OS_R1,   C_13,     C_56,     KC_LEFT, KC_DOWN, KC_RGHT
    ),

    // Capa 1: Fn1 Hold (Multimedia / Accesos / Control Brillo RGB)
    [1] = LAYOUT_iso_68(
        KC_GRV,  M_MAIL,  M_NAME,  M_SURN,  M_ADDR,  M_DNI,   M_TEL,   M_IBAN,  _______, _______, _______, _______,  _______,  _______,          _______,
        _______, M_GITS,  M_GITD,  M_GITA,  M_GITC,  M_COMP,  M_AGY,   _______, _______, _______, _______, _______,  _______,                    _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, NK_TOGG, _______, _______, _______, _______,            _______, RGB_U,
        _______, _______, _______,                            _______,                             RGB_O,   _______, _______,  _______, RGB_D,   _______
    ),

    // Capa 2: Caps Lock Hold (Speed Keys / Navegación)
    [2] = LAYOUT_iso_68(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  SK_BACK,          QK_BOOT,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                    SK_DEL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  SK_ENT,           _______,
        _______, _______, _______, _______, _______, SK_PAST, _______, _______, _______, _______, _______, _______,            SK_COUE,          SK_UP,
        _______, _______, _______,                            SK_COUS,                            _______, _______,  _______,  SK_LEFT, SK_DOWN, SK_RIGH
    ),

    // Capa 3: Fn1 Toggle (Tap)
    [3] = LAYOUT_iso_68(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                    _______,
        C_24,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______,
        _______, _______, _______,                            _______,                            _______, C_13,     C_56,     _______, _______, _______
    ),

    // Capa 4: Caps Lock Toggle (Tap - Edición / Contador)
    [4] = LAYOUT_iso_68(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                    _______,
        C_24,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  COUNT_E,          _______,
        _______, _______, UNDO,    CUT,     COPY,    PASTE,   _______, _______, _______, _______, _______, _______,            _______,          _______,
        COUNT,   KC_RGUI, _______,                            COUNT_S,                            _______, C_13,     C_56,     _______, _______, _______
    ),

    // Capa 5: Fn2 Hold (Teclas F1-F12)
    [5] = LAYOUT_iso_68(
        KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                    _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______,
        _______, _______, _______,                            _______,                            _______, _______,  _______,  _______, _______, _______
    ),

    // Capa 6: Fn2 Toggle (Tap)
    [6] = LAYOUT_iso_68(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,                    _______,
        C_24,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______,          _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,            _______, _______,
        _______, _______, _______,                            _______,                            _______, C_13,     C_56,     _______, _______, _______
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(_______, _______) },
    [2] = { ENCODER_CCW_CW(SK_PGUP, SK_PGDN) },
    [3] = { ENCODER_CCW_CW(_______, _______) },
    [4] = { ENCODER_CCW_CW(_______, _______) },
    [5] = { ENCODER_CCW_CW(_______, _______) },
    [6] = { ENCODER_CCW_CW(_______, _______) },
};
#endif
