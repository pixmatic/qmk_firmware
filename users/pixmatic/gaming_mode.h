#pragma once
#include "quantum.h"

// ------------------------------------------------------------------------------
// Modo de funcionamiento del teclado (default / gaming)
// ------------------------------------------------------------------------------
// El Keychron Q2 lleva un interruptor deslizante físico (el que en el firmware
// original de Keychron alternaba entre las capas de macOS y Windows). Está
// cableado a la matriz en la posición [4, 4] y QMK lo expone como DIP switch
// número 0 (ver "dip_switch" en keyboards/keychron/q2/info.json).
//
// Aquí lo reutilizamos como selector de modo global:
//   - Interruptor a la IZQUIERDA -> modo default (pixmatic_gaming_mode = false)
//   - Interruptor a la DERECHA   -> modo gaming  (pixmatic_gaming_mode = true)

// Índice del DIP switch que actúa como selector de modo.
#ifndef PIXMATIC_MODE_DIP_INDEX
#    define PIXMATIC_MODE_DIP_INDEX 0
#endif

// Estado eléctrico del DIP que se corresponde con el modo gaming.
// Si al probarlo en el teclado los modos salen invertidos, basta con
// redefinir esta macro a `false` en users/pixmatic/config.h.
#ifndef PIXMATIC_DIP_GAMING_ACTIVE_STATE
#    define PIXMATIC_DIP_GAMING_ACTIVE_STATE true
#endif

// Estado actual del modo. Lo consultan los módulos que se comportan distinto
// según el modo (de momento, la selección del algoritmo de debounce).
extern bool pixmatic_gaming_mode;
