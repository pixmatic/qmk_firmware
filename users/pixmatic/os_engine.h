#pragma once

#include "quantum.h"

// ==============================================================================
// OS ENGINE - SISTEMA DE MAPEO DINÁMICO DE TECLAS SEGÚN EL SISTEMA OPERATIVO
// ==============================================================================
// Este módulo permite definir teclas virtuales que emiten un código físico diferente
// dependiendo del Sistema Operativo detectado en tiempo de ejecución.
// ==============================================================================

// Estructura que define el mapeo de una tecla virtual.
typedef struct {
    uint16_t custom_keycode;    // El código de la tecla virtual del keymap (ej. OS_L1)
    uint16_t win_linux_keycode; // Código físico que se enviará en Windows y Linux (ej. KC_LCTL)
    uint16_t mac_keycode;       // Código físico que se enviará en macOS / iOS (ej. KC_LGUI)
} os_key_mapping_t;

// ------------------------------------------------------------------------------
// LISTA CENTRALIZADA DE TECLAS (Patrón X-Macro)
// ------------------------------------------------------------------------------
// Para añadir, eliminar o modificar teclas, solo edita esta lista.
// Formato: X(nombre_tecla_virtual, equivalente_win_linux, equivalente_mac)
// ------------------------------------------------------------------------------
#define OS_KEYS_LIST           \
    X(OS_L1, KC_LCTL, KC_LGUI) \
    X(OS_L2, KC_LGUI, KC_LCTL) \
    X(OS_L3, KC_LALT, KC_LGUI) \
    X(OS_R1, KC_RALT, KC_RALT) \
    X(OS_R2, KC_RGUI, KC_RGUI) \
    X(OS_R3, KC_RCTL, KC_RCTL)

#ifdef OS_DETECTION_ENABLE
// ==============================================================================
// CONFIGURACIÓN ACTIVA: Detección dinámica habilitada por hardware (USB)
// ==============================================================================

// Generamos el enum secuencial de las teclas virtuales de forma que no colisionen.
// Empezamos en SAFE_RANGE para garantizar que no toquen keycodes nativos de QMK.
enum os_engine_keycodes {
    OS_START_RANGE = SAFE_RANGE - 1,
#    define X(custom, win, mac) custom,
    OS_KEYS_LIST
#    undef X
        OS_ENGINE_SAFE_RANGE // Rango seguro para que el keymap.c declare sus propios keycodes
};

// Generamos el array estático de equivalencias que leerá la lógica de os_engine.c.
// Usamos __attribute__((unused)) para evitar que el compilador lance advertencias de
// "variable declarada pero no usada" en otros ficheros que incluyan esta cabecera (como pixmatic.c).
static const os_key_mapping_t os_key_mappings[] __attribute__((unused)) = {
#    define X(custom, win, mac) {custom, win, mac},
    OS_KEYS_LIST
#    undef X
};

// Macro para calcular dinámicamente la cantidad de teclas mapeadas
#    define OS_KEY_MAPPINGS_COUNT (sizeof(os_key_mappings) / sizeof(os_key_mapping_t))

/**
 * Intercepta y procesa las teclas virtuales de os_engine.
 * @param keycode El código de tecla pulsado.
 * @param record  El registro del evento de pulsación/liberación de QMK.
 * @return true si la tecla fue interceptada y procesada por el engine,
 *         false si no es una tecla de os_engine y QMK debe seguir procesándola normalmente.
 */
bool process_os_engine(uint16_t keycode, keyrecord_t *record);

#else
// ==============================================================================
// CONFIGURACIÓN PASIVA: Detección de OS deshabilitada (Optimizada para espacio)
// ==============================================================================
// Para ahorrar ciclos de CPU y memoria flash, las teclas virtuales se remapean
// estáticamente en tiempo de compilación a sus equivalencias por defecto de Windows/Linux.

enum os_engine_keycodes {
#    define X(custom, win, mac) custom = win,
    OS_KEYS_LIST
#    undef X
        OS_ENGINE_SAFE_RANGE = SAFE_RANGE // El rango seguro de usuario no se desplaza
};

// El procesador se redefine como una macro constante que devuelve false (no-op).
// Esto permite que el optimizador del compilador elimine todo el código muerto del binario.
#    define process_os_engine(keycode, record) false
#endif
