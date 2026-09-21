// Espacio de usuario centralizado para pixmatic
#define PIXMATIC_C
#include "quantum.h"
#include "os_engine.h"
#include "pixmatic.h"
#include "virg.h"
#include "gaming_mode.h"

// Personalizar el Tapping Term por tecla
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return TAPPING_TERM; // Valor global por defecto para el resto de teclas
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CLEAN_BOOTLOADER_JUMP_ENABLE
    if (keycode == QK_BOOT) {
        if (record->event.pressed) {
#    if defined(PROTOCOL_CHIBIOS)
            // Desconectar limpiamente el bus USB y dar tiempo al host para desmontar la sesión HID
            usbDisconnectBus(&USBD1);
            wait_ms(250);
#    endif
            bootloader_jump();
        }
        return false;
    }
#endif

    if (!process_shift_caps(keycode, record)) {
        return false;
    }
    // Los acentos muertos solo se emulan en modo default y sobre hosts que no los
    // resuelven de forma nativa. Esa logica consume pulsaciones y emite secuencias
    // (retroceso + acento), inadecuado cuando esas teclas son controles de juego.
    if (!pixmatic_gaming_mode && !host_is_apple() && !process_dead_tilde(keycode, record)) {
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
