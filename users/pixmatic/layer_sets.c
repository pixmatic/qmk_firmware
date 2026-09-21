// Despachador de juegos de capas (default / gaming).
//
// Toda busqueda de keycode en QMK pasa por keycode_at_keymap_location() y, para
// el encoder, por keycode_at_encodermap_location(). Ambas estan marcadas weak en
// quantum/keymap_introspection.c, asi que redefinirlas aqui basta para elegir el
// juego de capas segun el modo activo, sin tocar la pila de capas, los Tap Dance
// ni process_record.
//
// El despachador vive en el espacio de usuario y no en keymap.c porque ese
// fichero se incluye textualmente dentro de keymap_introspection.c: definir ahi
// estas funciones chocaria con su definicion weak en la misma unidad de
// compilacion.

#include "quantum.h"
#include "keymap_introspection.h"
#include "gaming_mode.h"

// Arrays del juego gaming, definidos en el fichero de capas del keymap.
extern const uint16_t keymaps_gaming[][MATRIX_ROWS][MATRIX_COLS];

uint16_t keycode_at_keymap_location(uint8_t layer_num, uint8_t row, uint8_t column) {
    if (!pixmatic_gaming_mode) {
        return keycode_at_keymap_location_raw(layer_num, row, column);
    }
    // Ambos juegos tienen el mismo numero de capas (comprobado con STATIC_ASSERT
    // en keymap.c), asi que el recuento del juego default sirve para los dos.
    if (layer_num < keymap_layer_count_raw() && row < MATRIX_ROWS && column < MATRIX_COLS) {
        return pgm_read_word(&keymaps_gaming[layer_num][row][column]);
    }
    return KC_TRNS;
}

#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)

extern const uint16_t encoder_map_gaming[][NUM_ENCODERS][NUM_DIRECTIONS];

uint16_t keycode_at_encodermap_location(uint8_t layer_num, uint8_t encoder_idx, bool clockwise) {
    if (!pixmatic_gaming_mode) {
        return keycode_at_encodermap_location_raw(layer_num, encoder_idx, clockwise);
    }
    if (layer_num < encodermap_layer_count_raw() && encoder_idx < NUM_ENCODERS) {
        return pgm_read_word(&encoder_map_gaming[layer_num][encoder_idx][clockwise ? 0 : 1]);
    }
    return KC_TRNS;
}

#endif // ENCODER_ENABLE && ENCODER_MAP_ENABLE
