#pragma once
#include "quantum.h"

// Mínimo común para Tap Dance
enum {
    TD_LSFT_CAPS = 0,
    TD_RSFT_CAPS
};

// Alias para la tecla de Tap Dance Shift
#define TD_SHIF TD(TD_LSFT_CAPS)

#if !defined(PIXMATIC_C)
#    include "caps_lock.c"
#endif

#include "macros.h"
