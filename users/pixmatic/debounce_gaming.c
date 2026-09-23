// Algoritmo de debounce del MODO GAMING: asym_eager_defer_pk.
//
// Registra la pulsación de forma inmediata (eager) y solo difiere la suelta,
// por tecla, lo que reduce la latencia percibida al pulsar. Misma técnica de
// renombrado que en debounce_default.c para poder enlazar ambos algoritmos.

#include "gaming_mode.h"

// Milisegundos de debounce del modo gaming.
#ifndef PIXMATIC_DEBOUNCE_GAMING
#    define PIXMATIC_DEBOUNCE_GAMING 5
#endif

#undef DEBOUNCE
#define DEBOUNCE PIXMATIC_DEBOUNCE_GAMING

#define debounce_init pixmatic_debounce_gaming_init
#define debounce      pixmatic_debounce_gaming

#include "quantum/debounce/asym_eager_defer_pk.c"

#undef debounce
#undef debounce_init
