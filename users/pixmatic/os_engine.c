#include "os_engine.h"

// Toda la lógica de ejecución del archivo se compila condicionalmente.
// Si OS_DETECTION_ENABLE = no en rules.mk, este archivo se compilará vacío (0 bytes de impacto).
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"

// Historial de teclas físicas actualmente pulsadas.
// Almacena qué código físico simulamos al presionar una tecla virtual específica.
// Esto garantiza que al soltar la tecla virtual liberemos EXACTAMENTE el mismo keycode físico
// que se registró al presionarla, incluso si el SO detectado cambia en mitad de la pulsación
// (por ejemplo, al cambiar de máquina mediante un switch KVM).
static uint16_t active_mapped_keycodes[OS_KEY_MAPPINGS_COUNT] = {0};

bool process_os_engine(uint16_t keycode, keyrecord_t *record) {
    // Buscamos si el keycode interceptado pertenece a nuestras teclas de os_engine
    for (size_t i = 0; i < OS_KEY_MAPPINGS_COUNT; i++) {
        if (os_key_mappings[i].custom_keycode == keycode) {
            if (record->event.pressed) {
                // ---------------------------------------------------------
                // EVENTO: Tecla presionada (Keydown)
                // ---------------------------------------------------------
                // 1. Preguntamos al driver USB de QMK qué SO tiene el host
                os_variant_t host_os = detected_host_os();
                uint16_t     target_keycode;

                // 2. Elegimos la equivalencia física según el SO
                if (host_os == OS_MACOS || host_os == OS_IOS) {
                    target_keycode = os_key_mappings[i].mac_keycode;
                } else {
                    // Windows, Linux o estado indeterminado (OS_UNSURE) usan el mapeo por defecto
                    target_keycode = os_key_mappings[i].win_linux_keycode;
                }

                // 3. Guardamos qué tecla física estamos enviando para poder liberarla después
                active_mapped_keycodes[i] = target_keycode;

                // 4. Mandamos la señal de tecla física presionada al host
                register_code16(target_keycode);
            } else {
                // ---------------------------------------------------------
                // EVENTO: Tecla liberada (Keyup)
                // ---------------------------------------------------------
                // Recuperamos el keycode exacto que enviamos en el keydown
                uint16_t target_keycode = active_mapped_keycodes[i];

                if (target_keycode != 0) {
                    // Enviamos señal de liberación física de la tecla guardada
                    unregister_code16(target_keycode);
                    active_mapped_keycodes[i] = 0; // Limpiamos el estado
                } else {
                    // Mecanismo de seguridad (Fallback): Si por alguna razón perdimos el estado,
                    // recalculamos el valor dinámicamente en caliente para evitar que la tecla se quede "atascada".
                    os_variant_t host_os = detected_host_os();
                    if (host_os == OS_MACOS || host_os == OS_IOS) {
                        target_keycode = os_key_mappings[i].mac_keycode;
                    } else {
                        target_keycode = os_key_mappings[i].win_linux_keycode;
                    }
                    unregister_code16(target_keycode);
                }
            }
            return true; // Indicamos a QMK que procesamos este keycode y que no continúe su lógica por defecto
        }
    }
    return false; // No es una tecla administrada por os_engine, continuar flujo normal
}
#endif
