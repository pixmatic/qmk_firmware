# Reglas del Proyecto QMK Firmware (Fork)

## Contexto del Proyecto
- Este proyecto es un fork del firmware QMK para configurar dos teclados: **Keychron K8 Pro** y **Keychron Q2**.
- Para el **Keychron K8 Pro**, no se utiliza el firmware del fork de Keychron, sino el firmware **vanilla** de QMK sin soporte para Bluetooth. Se ha creado el teclado de cero copiando los datos de hardware.
- Para el **Keychron Q2**, el modelo utilizado es la variante **ISO Encoder** (`keychron/q2/iso_encoder`). Cualquier ajuste, keymap o configuración para el Q2 debe realizarse siempre sobre esta variante.
- Para ambos teclados se utilizará **siempre** el keymap **`pixmatic`**.
- Por defecto se trabajará **siempre** con el **Keychron Q2 (ISO Encoder)**, el keymap **`pixmatic`** y el espacio de usuario **`users/pixmatic/`**, salvo que se indique expresamente lo contrario.

## Organización del Código
- **Espacio de usuario (`users/pixmatic/`)**: Toda la configuración de usuario debe ir en este directorio.
- **Teclados (`keyboards/`)**: En este directorio únicamente se ubicarán los keymaps (específicamente `pixmatic`) y la configuración específica de cada teclado.

## Compilación y Pruebas
- Para pruebas de compilación, se deben utilizar los comandos con el keymap `pixmatic`:
  ```bash
  # Keychron K8 Pro
  qmk compile -kb keychron/k8_pro -km pixmatic

  # Keychron Q2 (ISO Encoder)
  qmk compile -kb keychron/q2/iso_encoder -km pixmatic
  ```

## Restricciones de Comandos Git
- Está **estrictamente prohibido** utilizar comandos git de escritura (por ejemplo, `git commit`, `git reset`, cambios de rama, etc.), **excepto si el usuario lo solicita**.

## Principios de Diseño del Firmware
- Mantener el firmware **minimalista y ligero**, utilizando el mínimo número de opciones necesarias.

## Propuestas de Nuevas Funcionalidades
- Si el usuario propone implementar una funcionalidad similar a alguna de las funciones que ya incluye el firmware (las cuales se configuran en `rules.mk`), se le debe dar la opción de:
  1. Habilitarla y modificarla (si hace falta).
  2. Crearla desde cero.
- Cada propuesta de nueva funcionalidad similar a una nativa debe venir con una recomendación sobre cuál de las dos opciones elegir.
