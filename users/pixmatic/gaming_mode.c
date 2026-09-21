#include "gaming_mode.h"

// Arrancamos siempre en modo default. QMK fuerza una primera lectura del DIP
// switch durante el arranque (dip_switch_read(true)), así que este valor se
// corrige solo si el interruptor está en la posición de gaming.
bool pixmatic_gaming_mode = false;

#ifdef DIP_SWITCH_ENABLE

// Gancho de usuario invocado desde dip_switch_update_kb() (keyboards/keychron/q2/q2.c)
// cada vez que cambia la posición del interruptor.
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == PIXMATIC_MODE_DIP_INDEX) {
        pixmatic_gaming_mode = (active == PIXMATIC_DIP_GAMING_ACTIVE_STATE);
    }
    return true;
}

#endif // DIP_SWITCH_ENABLE
