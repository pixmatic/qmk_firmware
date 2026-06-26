#pragma once

#include "quantum.h"

// ==============================================================================
// GESTIÓN DE RANGOS DE KEYCODES PARA LAS MACROS
// ==============================================================================
// Calculamos el inicio del rango seguro de forma autónoma.
// Si el OS Engine está habilitado, reserva 6 keycodes (OS_L1 a OS_R3) después de SAFE_RANGE.
// ==============================================================================
#ifdef OS_DETECTION_ENABLE
#   define MACROS_START_RANGE (SAFE_RANGE + 6)
#else
#   define MACROS_START_RANGE SAFE_RANGE
#endif

// ==============================================================================
// 1. MACROS DE TIPO STRING (Envío de cadenas de texto)
// ==============================================================================
#define STRING_LIST \
    X(M_MAIL, "***REMOVED***") \
    X(M_NAME, "***REMOVED***") \
    X(M_SURN, "***REMOVED***") \
    X(M_ADDR, "***REMOVED***") \
    X(M_GITS, "git status\n") \
    X(M_GITD, "git diff\n") \
    X(M_GITA, "git add .\n") \
    X(M_GITC, "git commit -m \"\"") \
    X(M_COMP, "***REMOVED***")

// ==============================================================================
// 2. GENERACIÓN AUTOMÁTICA DE KEYCODES
// ==============================================================================
enum custom_macro_keycodes {
    MACRO_START_RANGE = MACROS_START_RANGE - 1,

    // Expandimos las macros de tipo string
#define X(name, string) name,
    STRING_LIST
#undef X

    // (Otros tipos de listas de macros se expandirán aquí en el futuro)

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
