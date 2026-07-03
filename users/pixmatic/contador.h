#pragma once
#include "quantum.h"
#include "speed_key.h"

// Rango de keycodes del contador
enum contador_keycodes {
    COUNT = SPEED_KEY_SAFE_RANGE,
    CONTADOR_SAFE_RANGE
};

/**
 * Intercepta y procesa la tecla COUNT.
 * @param keycode El código de tecla pulsado.
 * @param record  El registro del evento de pulsación/liberación de QMK.
 * @return true si la tecla fue COUNT y se procesó, false en caso contrario.
 */
bool process_contador(uint16_t keycode, keyrecord_t *record);

/**
 * Incrementa el contador y envía el número.
 */
void contador_incrementar_y_enviar(void);
