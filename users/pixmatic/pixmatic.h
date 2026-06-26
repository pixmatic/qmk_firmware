#pragma once
#include "quantum.h"

// Mínimo común para Tap Dance
#ifdef TAP_DANCE_ENABLE
enum {
    TD_LSFT_CAPS = 0,
    TD_RSFT_CAPS
};

// Alias para la tecla de Tap Dance Shift
#    define TD_SHIF TD(TD_LSFT_CAPS)

#    if !defined(PIXMATIC_C)
#        include "caps_lock.c"
#    endif
#else
// Si Tap Dance está desactivado, TD_SHIF se comporta como un Shift normal
#    define TD_SHIF KC_LSFT
#endif

#include "macros.h"
