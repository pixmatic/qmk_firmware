#include "virg.h"
#include "os_detection.h"

// Enumeración para representar los distintos acentos muertos
typedef enum {
    DEAD_NONE = 0,
    DEAD_TILDE,      // ~ (AltGr + 4)
    DEAD_ACUTE,      // ´ (KC_QUOT)
    DEAD_GRAVE,      // ` (KC_LBRC)
    DEAD_CIRCUMFLEX, // ^ (Shift + KC_LBRC)
    DEAD_DIAERESIS   // ¨ (Shift + KC_QUOT)
} dead_accent_t;

// Variables de estado
static dead_accent_t active_dead_accent = DEAD_NONE;
static uint16_t dead_accent_timer = 0;

// Determinar si una tecla es un modificador, cambio de capa o tecla de tap-dance
// para evitar que reinicie el estado de la tecla muerta
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

// Comprueba si una vocal es compatible con el acento activo
static bool is_compatible_vowel(dead_accent_t accent, uint16_t keycode, bool has_shift) {
    switch (accent) {
        case DEAD_TILDE:
            return (keycode == KC_A || keycode == KC_O);
        case DEAD_ACUTE:
            return (keycode == KC_A || keycode == KC_E || keycode == KC_I || keycode == KC_O || keycode == KC_U || keycode == KC_Y);
        case DEAD_GRAVE:
        case DEAD_CIRCUMFLEX:
            return (keycode == KC_A || keycode == KC_E || keycode == KC_I || keycode == KC_O || keycode == KC_U);
        case DEAD_DIAERESIS:
            if (keycode == KC_Y) {
                return !has_shift; // Sólo compatible si no tiene Shift (minúscula 'ÿ')
            }
            return (keycode == KC_A || keycode == KC_E || keycode == KC_I || keycode == KC_O || keycode == KC_U);
        default:
            return false;
    }
}

bool process_dead_tilde(uint16_t keycode, keyrecord_t *record) {
    // Si hay un acento activo, verificar si ha expirado el tiempo de 1.5 segundos
    if (active_dead_accent != DEAD_NONE) {
        if (timer_elapsed(dead_accent_timer) > 1500) {
            active_dead_accent = DEAD_NONE;
        }
    }

    uint8_t current_mods = get_mods();
    bool has_shift = (current_mods & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT))) != 0;
    bool has_altgr = (current_mods & MOD_BIT(KC_RALT)) != 0;
    bool has_other_mods = (current_mods & (MOD_BIT(KC_LALT) | MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL) | MOD_BIT(KC_LGUI) | MOD_BIT(KC_RGUI))) != 0;

    // Detectar las tildes basadas en un teclado con mapeo lógico español (ES)
    bool is_tilde = (keycode == RALT(KC_4)) || (keycode == KC_4 && has_altgr && !has_shift && !has_other_mods);
    bool is_acute = (keycode == KC_QUOT && !has_shift && !has_altgr && !has_other_mods);
    bool is_diaeresis = (keycode == LSFT(KC_QUOT)) || (keycode == KC_QUOT && has_shift && !has_altgr && !has_other_mods);
    bool is_grave = (keycode == KC_LBRC && !has_shift && !has_altgr && !has_other_mods);
    bool is_circumflex = (keycode == LSFT(KC_LBRC)) || (keycode == KC_LBRC && has_shift && !has_altgr && !has_other_mods);

    if (active_dead_accent != DEAD_NONE) {
        if (record->event.pressed) {
            // Ignorar modificadores y controles de capa
            if (is_ignored_key(keycode)) {
                return true;
            }

            dead_accent_t accent = active_dead_accent;
            active_dead_accent = DEAD_NONE; // Resetear siempre al presionar una tecla no ignorada

            if (is_compatible_vowel(accent, keycode, has_shift)) {
                uint8_t real_mods = get_mods();
                clear_mods();

                // 1. Borrar la tilde visual (+ espacio) que ya habíamos pintado en el host
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);

                // 2. Enviar la tecla muerta correspondiente sin espacio
                switch (accent) {
                    case DEAD_TILDE:
                        register_code16(RALT(KC_4));
                        unregister_code16(RALT(KC_4));
                        break;
                    case DEAD_ACUTE:
                        register_code(KC_QUOT);
                        unregister_code(KC_QUOT);
                        break;
                    case DEAD_GRAVE:
                        register_code(KC_LBRC);
                        unregister_code(KC_LBRC);
                        break;
                    case DEAD_CIRCUMFLEX:
                        register_code16(LSFT(KC_LBRC));
                        unregister_code16(LSFT(KC_LBRC));
                        break;
                    case DEAD_DIAERESIS:
                        register_code16(LSFT(KC_QUOT));
                        unregister_code16(LSFT(KC_QUOT));
                        break;
                    default:
                        break;
                }

                // 3. Restaurar modificadores para que la vocal se envíe correctamente
                set_mods(real_mods);

                // Devolvemos true para dejar pasar la vocal
                return true;
            }
        }
    }

    // Detección inicial de la pulsación de la tilde
    if (is_tilde || is_acute || is_diaeresis || is_grave || is_circumflex) {
        if (record->event.pressed) {
            os_variant_t host_os = detected_host_os();
            // Aplicar únicamente si NO estamos en macOS o iOS
            if (host_os != OS_MACOS && host_os != OS_IOS) {
                uint8_t real_mods = get_mods();
                clear_mods();

                dead_accent_t accent = DEAD_NONE;
                if (is_tilde) {
                    register_code16(RALT(KC_4));
                    unregister_code16(RALT(KC_4));
                    accent = DEAD_TILDE;
                } else if (is_acute) {
                    register_code(KC_QUOT);
                    unregister_code(KC_QUOT);
                    accent = DEAD_ACUTE;
                } else if (is_diaeresis) {
                    register_code16(LSFT(KC_QUOT));
                    unregister_code16(LSFT(KC_QUOT));
                    accent = DEAD_DIAERESIS;
                } else if (is_grave) {
                    register_code(KC_LBRC);
                    unregister_code(KC_LBRC);
                    accent = DEAD_GRAVE;
                } else if (is_circumflex) {
                    register_code16(LSFT(KC_LBRC));
                    unregister_code16(LSFT(KC_LBRC));
                    accent = DEAD_CIRCUMFLEX;
                }

                register_code(KC_SPC);
                unregister_code(KC_SPC);

                set_mods(real_mods);

                active_dead_accent = accent;
                dead_accent_timer = timer_read();
                return false; // Interceptado y gestionado por nosotros
            }
        }
    }

    return true; // Continuar flujo normal
}

