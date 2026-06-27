#include "speed_key.h"

// Variables de estado
static deferred_token speed_key_token = INVALID_DEFERRED_TOKEN;
static bool speed_key_active = false;
static uint16_t speed_key_code = KC_NO;

/**
 * @brief Función callback que se ejecuta de forma diferida en segundo plano
 * para simular la pulsación rápida y periódica de la tecla activa.
 */
uint32_t speed_key_callback(uint32_t trigger_time, void *cb_arg) {
    if (!speed_key_active) {
        speed_key_token = INVALID_DEFERRED_TOKEN;
        return 0; // Detiene la planificación repetitiva
    }

    // Registra (pulsa) y desregistra (suelta) el keycode
    tap_code16(speed_key_code);

    // Retorna el retraso en ms para volverse a ejecutar de forma diferida
    return SPEED_KEY_DELAY;
}

/**
 * @brief Intercepta y procesa las teclas de la ametralladora.
 */
bool process_speed_key(uint16_t keycode, keyrecord_t *record) {
    if (keycode >= RAPID_FIRE_START && keycode <= RAPID_FIRE_END) {
        uint16_t target_key = keycode - RAPID_FIRE_START;

        if (record->event.pressed) {
            // Guardamos el código de tecla a repetir y activamos la ráfaga
            speed_key_code = target_key;
            speed_key_active = true;

            // Disparamos la primera pulsación de manera inmediata
            tap_code16(speed_key_code);

            // Programamos las repeticiones consecutivas no bloqueantes
            speed_key_token = defer_exec(SPEED_KEY_DELAY, speed_key_callback, NULL);
        } else {
            // Detenemos la ráfaga únicamente si soltamos la misma tecla que está disparando
            if (speed_key_active && (target_key == speed_key_code)) {
                speed_key_active = false;
                if (speed_key_token != INVALID_DEFERRED_TOKEN) {
                    cancel_deferred_exec(speed_key_token);
                    speed_key_token = INVALID_DEFERRED_TOKEN;
                }
            }
        }
        return true; // Indicamos a QMK que procesamos este keycode
    }
    return false; // Continuar con el flujo normal de QMK
}
