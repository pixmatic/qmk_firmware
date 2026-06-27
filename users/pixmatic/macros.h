#pragma once

#include "quantum.h"

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
// reales de texto se definen en el fichero local "macros_content.h".
// Ejemplo de estructura de "macros_content.h":
//
//     #pragma once
//     #define VAL_M_MAIL "user@example.com"
//     #define VAL_M_NAME "JOHN"
//     #define VAL_M_SURN "DOE"
//     #define VAL_M_ADDR "123 Street"
//     #define VAL_M_GITS "git status\n"
//     #define VAL_M_GITD "git diff\n"
//     #define VAL_M_GITA "git add .\n"
//     #define VAL_M_GITC "git commit -m \"\""
//     #define VAL_M_COMP "qmk compile\n"
//
#include "macros_content.h"


#define STRING_LIST \
    X(M_MAIL, VAL_M_MAIL) \
    X(M_NAME, VAL_M_NAME) \
    X(M_SURN, VAL_M_SURN) \
    X(M_ADDR, VAL_M_ADDR) \
    X(M_GITS, VAL_M_GITS) \
    X(M_GITD, VAL_M_GITD) \
    X(M_GITA, VAL_M_GITA) \
    X(M_GITC, VAL_M_GITC) \
    X(M_COMP, VAL_M_COMP)

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
