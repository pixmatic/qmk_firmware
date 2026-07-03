#include "contador.h"

// Variable en memoria RAM para almacenar el contador
static uint16_t contador_val = 0;
// Temporizador para registrar el tiempo en que se presionó la tecla
static uint32_t contador_timer = 0;

/**
 * @brief Envía la representación en caracteres de un número entero.
 */
static void send_number(uint16_t num) {
    char buf[10];
    int i = 0;
    if (num == 0) {
        buf[i++] = '0';
    } else {
        uint16_t temp = num;
        while (temp > 0 && i < 9) {
            buf[i++] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    buf[i] = '\0';
    
    // Invertimos la cadena ya que se extrae del revés
    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
    
    send_string(buf);
}

void contador_incrementar_y_enviar(void) {
    contador_val++;
    send_number(contador_val);
}

bool process_contador(uint16_t keycode, keyrecord_t *record) {
    if (keycode == COUNT) {
        if (record->event.pressed) {
            // Guardar el tiempo inicial al presionar la tecla
            contador_timer = timer_read32();
        } else {
            // Al soltar, si estuvo presionada más de 500 ms (medio segundo),
            // reiniciamos el contador a 0.
            if (timer_elapsed32(contador_timer) >= 500) {
                contador_val = 0;
            } else {
                // Si fue una pulsación normal (corta), incrementamos y enviamos el número
                contador_incrementar_y_enviar();
            }
        }
        return true;
    }
    return false;
}
