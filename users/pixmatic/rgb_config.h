#pragma once

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

// ==============================================================================
// ALIAS DE TECLAS RGB
// ==============================================================================

#define RGB_O QK_RGB_MATRIX_TOGGLE     // Apagar/Encender matriz
#define RGB_U QK_RGB_MATRIX_VALUE_UP   // Subir brillo
#define RGB_D QK_RGB_MATRIX_VALUE_DOWN // Bajar brillo
