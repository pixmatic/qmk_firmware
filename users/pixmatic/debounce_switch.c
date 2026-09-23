// Despachador de debounce (DEBOUNCE_TYPE = custom).
//
// QMK llama a debounce() en cada escaneo de la matriz; aquí redirigimos la
// llamada al algoritmo que corresponda al modo activo:
//   - modo default -> sym_defer_g           (debounce_default.c)
//   - modo gaming  -> asym_eager_defer_pk   (debounce_gaming.c)
//
// Al cambiar de modo no hace falta reiniciar nada: ambos algoritmos derivan su
// trabajo de la comparación entre la matriz cruda y la ya filtrada, así que
// convergen por sí solos en el primer escaneo estable tras el cambio.

#include "debounce.h"
#include "gaming_mode.h"

void pixmatic_debounce_default_init(void);
bool pixmatic_debounce_default(matrix_row_t raw[], matrix_row_t cooked[], bool changed);

void pixmatic_debounce_gaming_init(void);
bool pixmatic_debounce_gaming(matrix_row_t raw[], matrix_row_t cooked[], bool changed);

void debounce_init(void) {
    pixmatic_debounce_default_init();
    pixmatic_debounce_gaming_init();
}

bool debounce(matrix_row_t raw[], matrix_row_t cooked[], bool changed) {
    if (pixmatic_gaming_mode) {
        return pixmatic_debounce_gaming(raw, cooked, changed);
    }
    return pixmatic_debounce_default(raw, cooked, changed);
}
