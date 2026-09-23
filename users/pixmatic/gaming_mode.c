#include "gaming_mode.h"

// Arrancamos siempre en modo default. QMK fuerza una primera lectura del DIP
// switch durante el arranque (dip_switch_read(true)), así que este valor se
// corrige solo si el interruptor está en la posición de gaming.
bool pixmatic_gaming_mode = false;

#ifdef DIP_SWITCH_ENABLE

// Deja el teclado en un estado limpio tras cambiar de modo.
//
// Con dos juegos de capas, el contenido de cada tecla y de cada capa cambia bajo
// los pies del usuario, asi que arrastrar el estado anterior da problemas:
//   - Una tecla que siguiera pulsada resolveria su release contra el juego
//     nuevo, pudiendo liberar un keycode distinto del que registro y dejando
//     teclas o modificadores colgados.
//   - Las capas toggle (3, 4 y 6) seguirian activas pero con otro contenido.
static void pixmatic_mode_changed(void) {
    clear_keyboard();
    layer_clear();

#ifdef RGB_MATRIX_ENABLE
    pixmatic_rgb_invalidate_led_cache();
#endif
}

// Gancho de usuario invocado desde dip_switch_update_kb() (keyboards/keychron/q2/q2.c)
// cada vez que cambia la posición del interruptor.
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == PIXMATIC_MODE_DIP_INDEX) {
        bool gaming = (active == PIXMATIC_DIP_GAMING_ACTIVE_STATE);

        if (gaming != pixmatic_gaming_mode) {
            pixmatic_gaming_mode = gaming;
            pixmatic_mode_changed();
        }
    }
    return true;
}

#endif // DIP_SWITCH_ENABLE
