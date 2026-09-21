#pragma once

#if !defined(__ASSEMBLER__)
#    include <stdint.h>
#endif

#ifdef RGB_MATRIX_ENABLE

// ==============================================================================
// CONFIGURACIÓN DE LA MATRIZ RGB POR DEFECTO PARA EL USUARIO 'pixmatic'
// ==============================================================================

/* Modo RGB por defecto: Fuerza al teclado a arrancar con un color estático fijo */
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR

/* Tono (Hue) por defecto: Determina el color inicial en el espectro HSV (0 = Rojo) */
#define RGB_MATRIX_DEFAULT_HUE 0

/* Saturación por defecto: Nivel de pureza del color inicial (0 = Blanco puro/sin color) */
#define RGB_MATRIX_DEFAULT_SAT 0

/* Brillo (Value) por defecto: Nivel de intensidad lumínica inicial (128 = 50% de potencia) */
#define RGB_MATRIX_DEFAULT_VAL 128

/* Tiempo de inactividad (Timeout): Apaga las luces RGB tras 2 minutos (120000 ms) sin pulsar teclas */
#define RGB_MATRIX_TIMEOUT 120000

#ifdef CAPS_LOCK_BLINK_ENABLE
// ==============================================================================
// INDICADOR DE BLOQUEO DE MAYÚSCULAS (PARPADEO DE TECLAS SHIFT)
// ==============================================================================

/* Velocidad de parpadeo de las teclas Shift cuando Bloq Mayús está activo (en ms) */
#    ifndef CAPS_LOCK_BLINK_INTERVAL
#        define CAPS_LOCK_BLINK_INTERVAL 500
#    endif

/* Color del parpadeo de las teclas Shift con Bloq Mayús activo (azul) */
#    ifndef CAPS_LOCK_BLINK_COLOR_R
#        define CAPS_LOCK_BLINK_COLOR_R 0
#    endif
#    ifndef CAPS_LOCK_BLINK_COLOR_G
#        define CAPS_LOCK_BLINK_COLOR_G 0
#    endif
#    ifndef CAPS_LOCK_BLINK_COLOR_B
#        define CAPS_LOCK_BLINK_COLOR_B 255
#    endif

#    if !defined(__ASSEMBLER__)
void process_caps_lock_blink(uint8_t val);
#    endif
#endif

// ==============================================================================
// INDICADOR DE MODO GAMING (TECLA ESCAPE)
// ==============================================================================

/* Color de la tecla Escape mientras el interruptor está en modo gaming (rojo) */
#ifndef GAMING_MODE_ESC_COLOR_R
#    define GAMING_MODE_ESC_COLOR_R 255
#endif
#ifndef GAMING_MODE_ESC_COLOR_G
#    define GAMING_MODE_ESC_COLOR_G 0
#endif
#ifndef GAMING_MODE_ESC_COLOR_B
#    define GAMING_MODE_ESC_COLOR_B 0
#endif

#if !defined(__ASSEMBLER__)
void process_gaming_mode_indicator(uint8_t val);
#endif

// ==============================================================================
// ALIAS DE TECLAS RGB
// ==============================================================================

#define RGB_O QK_RGB_MATRIX_TOGGLE     // Apagar/Encender matriz
#define RGB_U QK_RGB_MATRIX_VALUE_UP   // Subir brillo
#define RGB_D QK_RGB_MATRIX_VALUE_DOWN // Bajar brillo

#else

// ==============================================================================
// ALIAS DE CAÍDA (FALLBACK) CUANDO LA MATRIZ RGB ESTÁ DESHABILITADA
// ==============================================================================

#define RGB_O KC_NO
#define RGB_U KC_NO
#define RGB_D KC_NO

#endif
