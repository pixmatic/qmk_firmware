#include "quantum.h"
#include "rgb_config.h"
#include "gaming_mode.h"

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
            // Color del parpadeo (azul por defecto), escalado al brillo actual
            uint8_t r = (uint16_t)CAPS_LOCK_BLINK_COLOR_R * val / 255;
            uint8_t g = (uint16_t)CAPS_LOCK_BLINK_COLOR_G * val / 255;
            uint8_t b = (uint16_t)CAPS_LOCK_BLINK_COLOR_B * val / 255;

            if (lsft_led != NO_LED) {
                rgb_matrix_set_color(lsft_led, r, g, b);
            }
            if (rsft_led != NO_LED) {
                rgb_matrix_set_color(rsft_led, r, g, b);
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

static uint8_t esc_led       = NO_LED;
static bool    esc_led_found = false;

// Localiza automáticamente el índice del LED de la tecla Escape en la matriz
static void find_esc_led(void) {
    if (esc_led_found) return;
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        for (uint8_t c = 0; c < MATRIX_COLS; c++) {
            if (keymap_key_to_keycode(0, (keypos_t){.row = r, .col = c}) == KC_ESC) {
                esc_led = g_led_config.matrix_co[r][c];
            }
        }
    }
    esc_led_found = true;
}

// Tiñe la tecla Escape mientras el interruptor está en modo gaming, para poder
// distinguir el modo activo de un vistazo sin mirar el interruptor.
void process_gaming_mode_indicator(uint8_t val) {
    if (!pixmatic_gaming_mode) return;

    find_esc_led();
    if (esc_led == NO_LED) return;

    // Color del indicador (rojo por defecto), escalado al brillo actual
    rgb_matrix_set_color(esc_led, (uint16_t)GAMING_MODE_ESC_COLOR_R * val / 255, (uint16_t)GAMING_MODE_ESC_COLOR_G * val / 255, (uint16_t)GAMING_MODE_ESC_COLOR_B * val / 255);
}

#if RGB_MATRIX_TIMEOUT > 0
// En modo gaming la iluminación nunca debe apagarse por inactividad.
//
// RGB_MATRIX_TIMEOUT es una constante de compilación que QMK compara contra
// last_input_activity_elapsed(), así que no se puede desactivar en caliente.
// Lo que sí podemos es refrescar la marca de tiempo de actividad antes de que
// llegue a expirar, de modo que el timeout nunca se cumpla mientras el
// interruptor esté en modo gaming. En modo default no tocamos nada y la matriz
// se apaga con normalidad a los RGB_MATRIX_TIMEOUT ms.
void housekeeping_task_user(void) {
    if (pixmatic_gaming_mode && last_input_activity_elapsed() > (RGB_MATRIX_TIMEOUT / 2)) {
        uint32_t now = sync_timer_read32();
        set_activity_timestamps(now, now, now);
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

    // 3. Indicador del modo gaming en la tecla Escape.
    //    Se pinta el último para que no lo tape el color de capa de arriba.
    process_gaming_mode_indicator(val);

    return true; // Permitir que QMK procese otros indicadores si los hubiera
}
#endif
