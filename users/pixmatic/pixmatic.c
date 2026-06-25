// Espacio de usuario centralizado para pixmatic
#include "quantum.h"
#include "os_engine.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (process_os_engine(keycode, record)) {
        return false;
    }
    return true;
}
