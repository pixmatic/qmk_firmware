#include "virg.h"
#include "os_detection.h"

// Variable de estado para el comportamiento de tecla muerta de la tilde ~
static bool dead_tilde_active = false;

// Determinar si una tecla es un modificador, cambio de capa o tecla de tap-dance
// para evitar que reinicie el estado de la tecla muerta tilde
static bool is_ignored_key(uint16_t keycode) {
    // Modificadores básicos (Ctrl, Shift, Alt, GUI de ambos lados)
    if (keycode >= KC_LEFT_CTRL && keycode <= KC_RIGHT_GUI) {
        return true;
    }
    // Rango de Tap Dance
    if (keycode >= QK_TAP_DANCE && keycode <= QK_TAP_DANCE_MAX) {
        return true;
    }
    // Rango de Mod-Taps y Layer-Taps
    if (keycode >= QK_MOD_TAP && keycode <= QK_LAYER_TAP_MAX) {
        return true;
    }
    // Rango de Layer-Mods
    if (keycode >= QK_LAYER_MOD && keycode <= QK_LAYER_MOD_MAX) {
        return true;
    }
    // Rango de teclas de capa (TO, MO, DF, TG, OSL)
    if (keycode >= QK_TO && keycode <= QK_TOGGLE_LAYER_MAX) {
        return true;
    }
    if (keycode >= QK_ONE_SHOT_LAYER && keycode <= QK_ONE_SHOT_LAYER_MAX) {
        return true;
    }
    if (keycode >= QK_ONE_SHOT_MOD && keycode <= QK_ONE_SHOT_MOD_MAX) {
        return true;
    }
    if (keycode >= QK_LAYER_TAP_TOGGLE && keycode <= QK_LAYER_TAP_TOGGLE_MAX) {
        return true;
    }
    return false;
}

bool process_dead_tilde(uint16_t keycode, keyrecord_t *record) {
    // AltGr + 4 es ~ en la distribución de teclado española. En QMK esto se mapea como RALT(KC_4).
    // También validamos si se pulsa físicamente KC_4 mientras se sostiene AltGr (KC_RALT).
    bool is_tilde_pressed = (keycode == RALT(KC_4)) || (keycode == KC_4 && (get_mods() & MOD_BIT(KC_RALT)));

    if (dead_tilde_active) {
        if (record->event.pressed) {
            // Ignorar modificadores y controles de capa para no interferir al pulsar Shift + vocal
            if (is_ignored_key(keycode)) {
                return true;
            }

            dead_tilde_active = false;

            // Si es una vocal compatible con la tilde (A, O)
            if (keycode == KC_A || keycode == KC_O) {
                // Guardamos los modificadores físicos del usuario (por ejemplo, Shift)
                uint8_t real_mods = get_mods();
                clear_mods();

                // 1. Borrar la tilde visual (~ ) que ya habíamos pintado en el host
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);

                // 2. Enviar AltGr + 4 para indicarle al SO la tecla muerta (~ sin espacio)
                register_code16(RALT(KC_4));
                unregister_code16(RALT(KC_4));

                // 3. Restaurar modificadores para que la vocal se envíe correctamente (mayúscula/minúscula)
                set_mods(real_mods);

                // Devolvemos true para dejar pasar la pulsación de la vocal y que se combine con la tecla muerta en el SO
                return true;
            }
            // Si es cualquier otra tecla, el estado tilde muerta se desactiva y pasa normal
        }
    }

    if (is_tilde_pressed) {
        if (record->event.pressed) {
            os_variant_t host_os = detected_host_os();
            // Aplicar únicamente si NO estamos en macOS o iOS
            if (host_os != OS_MACOS && host_os != OS_IOS) {
                uint8_t real_mods = get_mods();
                clear_mods();

                // Enviar AltGr + 4 seguido de Espacio para forzar la impresión visual de la tilde
                register_code16(RALT(KC_4));
                unregister_code16(RALT(KC_4));
                register_code(KC_SPC);
                unregister_code(KC_SPC);

                set_mods(real_mods);

                dead_tilde_active = true;
                return false; // Interceptado y gestionado por nosotros
            }
        }
    }

    return true; // Continuar flujo normal
}
