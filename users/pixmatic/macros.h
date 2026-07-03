#pragma once

#include "quantum.h"

// ==============================================================================
// ALIAS DE EDICIÓN RÁPIDA (Ctrl+Z, Ctrl+X, Ctrl+C, Ctrl+V)
// ==============================================================================
#define UNDO  LCTL(KC_Z)
#define CUT   LCTL(KC_X)
#define COPY  LCTL(KC_C)
#define PASTE LCTL(KC_V)

// ==============================================================================
// GESTIÓN DE RANGOS DE KEYCODES PARA LAS MACROS
// ==============================================================================
// Calculamos el inicio del rango seguro de forma autónoma.
// Si el OS Engine está habilitado, reserva 6 keycodes (OS_L1 a OS_R3) después de SAFE_RANGE.
// ==============================================================================
#define MACROS_START_RANGE (SAFE_RANGE + 6)

// ==============================================================================
// 1. MACROS DE TIPO STRING (Envío de cadenas de texto)
// ==============================================================================
// Para no incluir strings privados en el repositorio, las variables
// reales de texto se definen en el fichero local "macros_content.h"
// usando la sintaxis de lista X-Macros.
//
// Ejemplo de estructura de "macros_content.h":
//
//     #pragma once
//     #define MACROS_LIST X(M_MAIL, "user@example.com") X(M_NAME, "JOHN")
//
#if __has_include("macros_content.h")
#    include "macros_content.h"
#else
#    define MACROS_LIST
#    error "Falta el fichero 'users/pixmatic/macros_content.h' (ver macros.h para ejemplo de estructura)"
#endif


// ==============================================================================
// 2. GENERACIÓN AUTOMÁTICA DE KEYCODES
// ==============================================================================
enum custom_macro_keycodes {
    MACRO_START_RANGE = MACROS_START_RANGE - 1,

    // Expandimos las macros
#define X(name, string) name,
    MACROS_LIST
#undef X

    // (Otros tipos de listas de macros se expandirán aquí en el futuro)
    COUNT_S,
    COUNT_E,

    MACRO_SAFE_RANGE
};

/**
 * Intercepta y procesa las macros personalizadas.
 * @param keycode El código de tecla pulsado.
 * @param record  El registro del evento de pulsación/liberación de QMK.
 * @return true si la tecla fue una macro procesada por este módulo,
 *         false de lo contrario.
 */
bool process_macros(uint16_t keycode, keyrecord_t *record);
