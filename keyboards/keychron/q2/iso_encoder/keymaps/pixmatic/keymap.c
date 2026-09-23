/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "os_engine.h"
#include "pixmatic.h"
#include "compiler_support.h"

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

// ==============================================================================
// NEXO DE LOS DOS JUEGOS DE CAPAS
// ==============================================================================
// Cada juego de capas vive en su propio fichero y se incluye aqui textualmente.
// Ambos deben declarar el mismo numero de capas y mantener la misma organizacion
// (roles de cada capa y modificadores), porque la pila de capas de QMK es unica
// y tanto los Tap Dance de mod_keys.c como los colores de capa del RGB usan
// numeros de capa fijos.
//
// El reparto entre uno y otro segun el modo activo lo hace el despachador de
// users/pixmatic/layer_sets.c. No puede estar en este fichero: keymap.c se
// incluye dentro de quantum/keymap_introspection.c, de modo que redefinir aqui
// keycode_at_keymap_location() chocaria con su definicion weak en esa misma
// unidad de compilacion.

#include "layers_default.c"

#ifdef DUAL_LAYER_SETS_ENABLE
#    include "layers_gaming.c"

// Un descuadre entre juegos haria inalcanzables las capas sobrantes, porque el
// numero de capas del firmware sale unicamente de sizeof(keymaps).
STATIC_ASSERT(sizeof(keymaps) == sizeof(keymaps_gaming), "Los dos juegos de capas deben tener el mismo numero de capas");
#    if defined(ENCODER_MAP_ENABLE)
STATIC_ASSERT(sizeof(encoder_map) == sizeof(encoder_map_gaming), "Los dos encoder_map deben tener el mismo numero de capas");
#    endif
#endif
