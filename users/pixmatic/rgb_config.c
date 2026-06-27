#include "quantum.h"

#ifdef RGB_MATRIX_ENABLE
// Callback oficial de QMK para controlar indicadores RGB basados en el estado del teclado
bool rgb_matrix_indicators_user(void) {
    // Si la capa activa más alta es la 4 (activada por el tap de C_24 / Bloq Mayús), iluminamos en Rojo.
    // Si es la 3 (activada por el tap de C_13 / Fn), iluminamos en Verde.
    uint8_t highest_layer = get_highest_layer(layer_state);
    uint8_t val = rgb_matrix_get_val();

    if (highest_layer == 4) {
        rgb_matrix_set_color_all(val, 0, 0); // Rojo
    } else if (highest_layer == 3) {
        rgb_matrix_set_color_all(0, val, 0); // Verde
    }
    return true; // Permitir que QMK procese otros indicadores si los hubiera
}
#endif
