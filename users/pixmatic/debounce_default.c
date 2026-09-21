// Algoritmo de debounce del MODO DEFAULT: sym_defer_g (el de serie en QMK).
//
// Compila una copia privada de quantum/debounce/sym_defer_g.c renombrando sus
// dos símbolos públicos, de modo que pueda convivir en el mismo binario con el
// algoritmo del modo gaming. El despachador (debounce_switch.c) es quien decide
// cuál de los dos se ejecuta en cada escaneo de la matriz.

#include "gaming_mode.h"

// Milisegundos de debounce del modo default.
#ifndef PIXMATIC_DEBOUNCE_DEFAULT
#    define PIXMATIC_DEBOUNCE_DEFAULT 5
#endif

#undef DEBOUNCE
#define DEBOUNCE PIXMATIC_DEBOUNCE_DEFAULT

#define debounce_init pixmatic_debounce_default_init
#define debounce      pixmatic_debounce_default

#include "quantum/debounce/sym_defer_g.c"

#undef debounce
#undef debounce_init
