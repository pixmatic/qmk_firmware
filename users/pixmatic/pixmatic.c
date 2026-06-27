// Espacio de usuario centralizado para pixmatic
#define PIXMATIC_C
#include "quantum.h"
#include "os_engine.h"
#include "pixmatic.h"
#include "virg.h"

// Personalizar el Tapping Term por tecla
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD_SHIF:
            return 500; // 500 ms específicos para la doble pulsación del Shift
        default:
            return TAPPING_TERM; // Valor global por defecto para el resto de teclas
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_dead_tilde(keycode, record)) {
        return false;
    }
    if (process_os_engine(keycode, record)) {
        return false;
    }
    if (process_macros(keycode, record)) {
        return false;
    }
    return true;
}
