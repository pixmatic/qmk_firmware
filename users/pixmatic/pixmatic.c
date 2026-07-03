// Espacio de usuario centralizado para pixmatic
#define PIXMATIC_C
#include "quantum.h"
#include "os_engine.h"
#include "pixmatic.h"
#include "virg.h"

// Personalizar el Tapping Term por tecla
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return TAPPING_TERM; // Valor global por defecto para el resto de teclas
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_shift_caps(keycode, record)) {
        return false;
    }
    if (!process_dead_tilde(keycode, record)) {
        return false;
    }
    if (process_os_engine(keycode, record)) {
        return false;
    }
    if (process_macros(keycode, record)) {
        return false;
    }
    if (process_speed_key(keycode, record)) {
        return false;
    }
    if (process_contador(keycode, record)) {
        return false;
    }
    return true;
}

void tap_code16_delay(uint16_t code, uint16_t delay) {
    if (code >= SAFE_RANGE) {
        keyrecord_t record;
        record.event.pressed = true;
        record.event.time = timer_read();
        process_record_user(code, &record);
        
        if (delay > 0) {
            wait_ms(delay);
        }
        
        record.event.pressed = false;
        record.event.time = timer_read();
        process_record_user(code, &record);
    } else {
        register_code16(code);
        if (delay > 0) {
            wait_ms(delay);
        }
        unregister_code16(code);
    }
}
