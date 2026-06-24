# Reglas del Proyecto QMK Firmware (Fork)

## Contexto del Proyecto
- Este proyecto es un fork del firmware QMK para configurar dos teclados: **Keychron K8 Pro** y **Keychron Q2**.
- Para el **Keychron K8 Pro**, no se utiliza el firmware del fork de Keychron, sino el firmware **vanilla** de QMK sin soporte para Bluetooth. Se ha creado el teclado de cero copiando los datos de hardware.

## Organización del Código
- **Espacio de usuario (`users/pixmatic/`)**: Toda la configuración de usuario debe ir en este directorio.
- **Teclados (`keyboards/`)**: En este directorio únicamente se ubicarán los keymaps y la configuración específica de cada teclado.

## Compilación y Pruebas
- Para pruebas de compilación, se debe utilizar el comando:
  ```bash
  qmk compile -kb keychron/k8_pro -km pixmatic
  ```

## Restricciones de Comandos Git
- Está **estrictamente prohibido** utilizar comandos git de escritura (por ejemplo, `git commit`, `git reset`, cambios de rama, etc.).

## Principios de Diseño del Firmware
- Mantener el firmware **minimalista y ligero**, utilizando el mínimo número de opciones necesarias.

## Propuestas de Nuevas Funcionalidades
- Si el usuario propone implementar una funcionalidad similar a alguna de las funciones que ya incluye el firmware (las cuales se configuran en `rules.mk`), se le debe dar la opción de:
  1. Habilitarla y modificarla (si hace falta).
  2. Crearla desde cero.
- Cada propuesta de nueva funcionalidad similar a una nativa debe venir con una recomendación sobre cuál de las dos opciones elegir.
