#pragma once

#include "quantum.h"

/**
 * Procesa la lógica de tecla muerta para la tilde (virgulilla '~') en Windows/Linux.
 * @param keycode El código de tecla pulsado.
 * @param record  El registro del evento de pulsación/liberación de QMK.
 * @return true si la tecla debe seguir procesándose normalmente,
 *         false si la tecla fue totalmente gestionada y consumida.
 */
bool process_dead_tilde(uint16_t keycode, keyrecord_t *record);
