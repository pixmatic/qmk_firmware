#include "quantum.h"
#include "rgb_config.h"

#ifdef RGB_MATRIX_ENABLE

#ifdef CAPS_LOCK_BLINK_ENABLE
static uint8_t lsft_led = NO_LED;
static uint8_t rsft_led = NO_LED;
static bool    shift_leds_found = false;

// Localiza automáticamente los índices de los LEDs de Left Shift y Right Shift en la matriz
static void find_shift_leds(void) {
    if (shift_leds_found) return;
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            uint16_t kc = keymap_key_to_keycode(0, (keypos_t){.row = r, .col = c});
            if (kc == KC_LSFT) {
                lsft_led = g_led_config.matrix_co[r][c];
            } else if (kc == KC_RSFT) {
                rsft_led = g_led_config.matrix_co[r][c];
            }
        }
    }
    shift_leds_found = true;
}

// Función del espacio de usuario que gestiona el parpadeo de las teclas Shift al estar activo Bloq Mayús
void process_caps_lock_blink(uint8_t val) {
    if (host_keyboard_led_state().caps_lock) {
        find_shift_leds();
        bool is_blink_on = ((timer_read32() / CAPS_LOCK_BLINK_INTERVAL) % 2) == 0;

        if (is_blink_on) {
            if (lsft_led != NO_LED) {
                rgb_matrix_set_color(lsft_led, val, val, val);
            }
            if (rsft_led != NO_LED) {
                rgb_matrix_set_color(rsft_led, val, val, val);
            }
        } else {
            if (lsft_led != NO_LED) {
                rgb_matrix_set_color(lsft_led, 0, 0, 0);
            }
            if (rsft_led != NO_LED) {
                rgb_matrix_set_color(rsft_led, 0, 0, 0);
            }
        }
    }
}
#endif

// Callback oficial de QMK para controlar indicadores RGB basados en el estado del teclado
bool rgb_matrix_indicators_user(void) {
    uint8_t highest_layer = get_highest_layer(layer_state);
    uint8_t val           = rgb_matrix_get_val();

    // 1. Color según la capa toggle activa
    if (highest_layer == 4) {
        rgb_matrix_set_color_all(val, 0, 0); // Rojo (Caps Lock Toggle)
    } else if (highest_layer == 3) {
        rgb_matrix_set_color_all(0, val, 0); // Verde (Fn1 Toggle)
    } else if (highest_layer == 6) {
        rgb_matrix_set_color_all(0, 0, val); // Azul (Fn2 Toggle)
    }

#ifdef CAPS_LOCK_BLINK_ENABLE
    // 2. Efecto de parpadeo de los Shift si Bloq Mayús está activo
    process_caps_lock_blink(val);
#endif

    return true; // Permitir que QMK procese otros indicadores si los hubiera
}
#endif
