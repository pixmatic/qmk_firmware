#include "macros.h"
// Se incluye el mapa de traducción español para que SEND_STRING traduzca
// correctamente los caracteres como la '@' usando la distribución de teclado de España.
#include "sendstring_spanish.h"

/**
 * @brief Envía una cadena de texto almacenada en PROGMEM (memoria flash)
 * asegurando que el estado de Bloq Mayús no altere las mayúsculas/minúsculas.
 *
 * @param str Puntero a la cadena de texto en PROGMEM.
 */
static void send_progmem_string_with_caps_check(const char *str) {
    // 1. Consultamos el estado actual del LED de Bloq Mayús reportado por el SO.
    bool is_caps = host_keyboard_led_state().caps_lock;

    // 2. Si Bloq Mayús está activo, lo apagamos temporalmente para evitar
    // que invierta las mayúsculas y minúsculas del texto que vamos a escribir.
    if (is_caps) {
        tap_code(KC_CAPS);
        // Esperamos 80ms para dar tiempo a que la controladora USB del host
        // y el sistema operativo actualicen y apliquen el cambio de estado.
        wait_ms(10);
    }

    // 3. Escribimos la cadena de texto. Usamos send_string_P para indicar
    // al compilador que lea el string directamente desde la memoria Flash (PROGMEM),
    // lo cual ahorra memoria RAM valiosa en el microcontrolador.
    send_string_P(str);

    // 4. Si apagamos Bloq Mayús al inicio, procedemos a restaurar su estado.
    if (is_caps) {
        // Añadimos una pequeña espera de 150ms antes de pulsar Bloq Mayús de nuevo.
        // Esto evita que la pulsación del Caps Lock se solape con el procesamiento
        // de las últimas letras enviadas si el sistema operativo tiene latencia de entrada.
        wait_ms(10);
        tap_code(KC_CAPS);
    }
}

/**
 * @brief Intercepta y procesa las pulsaciones de teclas para ejecutar macros.
 *
 * @param keycode Código de la tecla pulsada.
 * @param record  Información del evento de pulsación (keydown/keyup).
 * @return true si la tecla fue procesada como una macro, false de lo contrario.
 */
bool process_macros(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // --- Procesamiento de macros de tipo String ---
        // Usamos el patrón X-Macro para expandir automáticamente todas las macros
        // definidas en la tabla STRING_LIST de macros.h. Esto evita repetir
        // bloques "case" idénticos y facilita el mantenimiento.
#define X(name, string) \
        case name: \
            if (record->event.pressed) { \
                /* PSTR(string) fuerza al compilador a ubicar el literal en PROGMEM */ \
                send_progmem_string_with_caps_check(PSTR(string)); \
            } \
            return true;
        STRING_LIST
#undef X

        // --- Procesamiento de otros tipos de macros en el futuro ---

        default:
            return false;
    }
}
