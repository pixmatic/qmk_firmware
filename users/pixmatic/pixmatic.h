#pragma once
#include "quantum.h"

#include "macros.h"
#include "speed_key.h"
#include "contador.h"

// Enumeración para identificar las acciones personalizadas de Tap Dance del espacio de usuario.
// Se usan como índices dentro del array `tap_dance_actions` en mod_keys.c.
enum {
    TD_C24_LAYER = 0, // Tap Dance para la tecla Caps Lock / C_24 (Alterna Capa 4, mantiene Capa 2)
    TD_C13_LAYER      // Tap Dance para la tecla Fn / C_13 (Alterna Capa 3, mantiene Capa 1)
};

// Redefinimos el comportamiento por defecto de _______ (transparencia) en los keymaps.
// Se mapea a KC_NO para evitar que las pulsaciones se propaguen inesperadamente hacia capas inferiores,
// asegurando que cada capa sea exclusiva y defina estrictamente sus funciones.
#undef _______
#define _______ KC_NO

// Rango inicial seguro para los keycodes personalizados del espacio de usuario de pixmatic.
enum pixmatic_keycodes {
    PIXMATIC_SAFE_RANGE = CONTADOR_SAFE_RANGE
};

// Alias de conveniencia para invocar las acciones Tap Dance desde la matriz de keymaps.
// C_24: Hold activa momentáneamente la Capa 2. Tap alterna (toggle) de manera persistente la Capa 4.
#define C_24 TD(TD_C24_LAYER)

// C_13: Hold activa momentáneamente la Capa 1. Tap alterna (toggle) de manera persistente la Capa 3.
#define C_13 TD(TD_C13_LAYER)

// Declara la función para la gestión del comportamiento inteligente de Caps Lock (activación al presionar ambos Shift).
bool process_shift_caps(uint16_t keycode, keyrecord_t *record);

// Inclusión del módulo de control de modificadores (mod_keys.c).
// Solo se incluye si no se está compilando desde el archivo fuente principal pixmatic.c,
// previniendo duplicados de símbolos durante la etapa de enlazado.
#if !defined(PIXMATIC_C)
#    include "mod_keys.c"
#endif

