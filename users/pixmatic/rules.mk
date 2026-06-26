# ==============================================================================
# CARACTERÍSTICAS DEL ESPACIO DE USUARIO 'pixmatic'
# ==============================================================================
# Se deshabilitan la gran mayoría de las características de QMK por defecto para
# obtener un firmware base minimalista, liviano y con menor consumo de memoria.
# Habilita lo que necesites de forma explícita conforme lo requieras.

# Incluir archivos de código fuente del espacio de usuario
SRC += pixmatic.c os_engine.c macros.c

# ------------------------------------------------------------------------------
# 1. Optimización y Depuración
# ------------------------------------------------------------------------------
LTO_ENABLE = yes                    # Habilitar Link-Time Optimization para reducir tamaño del binario
DEBUG_ENABLE = no                   # Deshabilitar depuración general
CONSOLE_ENABLE = no                 # Deshabilitar consola de depuración (ahorra mucho espacio)
COMMAND_ENABLE = no                 # Deshabilitar comandos interactivos de depuración (Magic keys)
VARIABLE_TRACE = no                 # Deshabilitar traza de variables
DEBUG_MATRIX_SCAN_RATE_ENABLE = no  # Deshabilitar depuración de tasa de escaneo de matriz
AUTOLOG_ENABLE = no                 # Deshabilitar registro automático de depuración

# ------------------------------------------------------------------------------
# 2. Características por Defecto en QMK (Forzadas a NO)
# ------------------------------------------------------------------------------
GRAVE_ESC_ENABLE = no               # Deshabilitar comportamiento Grave Escape (Esc/` compuestas)
MAGIC_ENABLE = no                   # Deshabilitar teclas mágicas (configuración al vuelo del hardware)
SEND_STRING_ENABLE = yes            # Habilitar envío de secuencias de texto macro
SPACE_CADET_ENABLE = no             # Deshabilitar Space Cadet (modificadores que actúan como paréntesis al tapear)
VELOCIKEY_ENABLE = no               # Deshabilitar ajuste dinámico de velocidad de efectos por pulsación
SWAP_HANDS_ENABLE = no              # Deshabilitar intercambio de manos (teclados de una sola mano)
KEYBOARD_SHARED_EP = no             # Deshabilitar endpoint USB compartido para el teclado
MOUSE_SHARED_EP = no                # Deshabilitar endpoint USB compartido para el ratón

# ------------------------------------------------------------------------------
# 3. Funciones de Teclado y Capas (QMK Features)
# ------------------------------------------------------------------------------
AUTO_SHIFT_ENABLE = no              # Deshabilitar Auto Shift (mantener pulsado para mayúsculas)
AUTOCORRECT_ENABLE = no             # Deshabilitar autocorrección de texto
CAPS_WORD_ENABLE = no               # Deshabilitar Caps Word (Caps Lock inteligente que se apaga al pulsar espacio)
COMBO_ENABLE = no                   # Deshabilitar combos de teclas
KEY_LOCK_ENABLE = no                # Deshabilitar Key Lock (bloqueo de teclas)
KEY_OVERRIDE_ENABLE = no            # Deshabilitar anulaciones de teclas (Key Overrides)
LAYER_LOCK_ENABLE = no              # Deshabilitar bloqueo de capas
LEADER_ENABLE = no                  # Deshabilitar tecla Leader (secuencias de teclas estilo Vim)
REPEAT_KEY_ENABLE = no              # Deshabilitar tecla Repeat (repite la última acción)
TAP_DANCE_ENABLE = yes              # Habilitar Tap Dance
TRI_LAYER_ENABLE = no               # Deshabilitar Tri-Layer (activar tercera capa al pulsar dos específicas)
WPM_ENABLE = no                     # Deshabilitar estimación de Words Per Minute (WPM)
UNICODE_ENABLE = no                 # Deshabilitar entrada de caracteres Unicode estándar
UNICODEMAP_ENABLE = no              # Deshabilitar mapeo de caracteres Unicode
UCIS_ENABLE = no                    # Deshabilitar sistema de entrada de caracteres Unicode (estilo diccionario)
UNICODE_COMMON = no                 # Deshabilitar funcionalidad común de Unicode
GERMAN_ENABLE = no                  # Deshabilitar soporte específico de idioma alemán
KEYLOGGER_ENABLE = no               # Deshabilitar registro interno de pulsaciones de teclas (Keylogger)
MACROS_ENABLED = no                 # Deshabilitar soporte básico de macros

# ------------------------------------------------------------------------------
# 4. Mapeo y Configuración Dinámica
# ------------------------------------------------------------------------------
DYNAMIC_KEYMAP_ENABLE = no          # Deshabilitar mapeo dinámico de teclas en EEPROM
DYNAMIC_MACRO_ENABLE = no           # Deshabilitar macros dinámicos (grabar macros al vuelo)
DYNAMIC_TAPPING_TERM_ENABLE = no    # Deshabilitar ajuste dinámico del Tapping Term
VIA_ENABLE = no                     # Deshabilitar compatibilidad con configurador VIA
VIAL_ENABLE = no                    # Deshabilitar compatibilidad con configurador VIAL

# ------------------------------------------------------------------------------
# 5. Hardware, Iluminación y Periféricos
# ------------------------------------------------------------------------------
BACKLIGHT_ENABLE = no               # Deshabilitar retroiluminación monocromática
RGBLIGHT_ENABLE = no                # Deshabilitar tiras de LED RGB (Underglow)
RGB_MATRIX_ENABLE = yes             # Habilitar matrices de LED RGB direccionables por tecla
LED_MATRIX_ENABLE = no              # Deshabilitar matrices de LED monocromáticas
SLEEP_LED_ENABLE = no               # Deshabilitar LED de estado en modo suspensión
ENCODER_ENABLE = no                 # Deshabilitar soporte para encoders rotativos
ENCODER_MAP_ENABLE = no             # Deshabilitar mapas de encoders
DIP_SWITCH_ENABLE = no              # Deshabilitar soporte para interruptores DIP
DIP_SWITCH_MAP_ENABLE = no          # Deshabilitar mapas de interruptores DIP
POINTING_DEVICE_ENABLE = no         # Deshabilitar soporte para trackballs/sensores ópticos
PS2_ENABLE = no                     # Deshabilitar periféricos PS/2 genéricos
PS2_MOUSE_ENABLE = no               # Deshabilitar emulación de ratón PS/2
JOYSTICK_ENABLE = no                # Deshabilitar soporte para Joystick analógicos/digitales
USBPD_ENABLE = no                   # Deshabilitar USB Power Delivery
HAPTIC_ENABLE = no                  # Deshabilitar feedback háptico (motores ERM/LRA)
BATTERY_ENABLE = no                 # Deshabilitar monitor de nivel de batería
OLED_ENABLE = no                    # Deshabilitar soporte para pantallas OLED
ST7565_ENABLE = no                  # Deshabilitar soporte para pantallas LCD ST7565
HD44780_ENABLE = no                 # Deshabilitar soporte para pantallas LCD HD44780
LCD_BACKLIGHT_ENABLE = no           # Deshabilitar retroiluminación de pantalla LCD
LCD_ENABLE = no                     # Deshabilitar soporte genérico para pantallas LCD
LED_BACK_ENABLE = no                # Deshabilitar retroiluminación posterior de LEDs
LED_UNDERGLOW_ENABLE = no           # Deshabilitar luces de retroiluminación inferior
LED_TABLES = no                     # Deshabilitar tablas de corrección gamma para LEDs
ENCODER_ENABLE_CUSTOM = no          # Deshabilitar controlador personalizado de encoders
DISABLE_PROMICRO_LEDs = no          # Deshabilitar LEDs internos en placas Pro Micro

# ------------------------------------------------------------------------------
# 6. Multimedia y Conectividad
# ------------------------------------------------------------------------------
AUDIO_ENABLE = no                   # Deshabilitar audio por altavoz incorporado
MIDI_ENABLE = no                    # Deshabilitar interfaz de control MIDI
STENO_ENABLE = no                   # Deshabilitar taquigrafía (Stenography)
SECURE_ENABLE = no                  # Deshabilitar API de desbloqueo de seguridad del teclado
BLUETOOTH_ENABLE = no               # Deshabilitar conectividad Bluetooth
VIRTSER_ENABLE = no                 # Deshabilitar puerto serie virtual USB
QUANTUM_PAINTER_ENABLE = no         # Deshabilitar motor gráfico Quantum Painter (pantallas color/SPI)
DIGITIZER_ENABLE = no               # Deshabilitar soporte para digitalizador (lápiz/pantalla táctil)
PROGRAMMABLE_BUTTON_ENABLE = no     # Deshabilitar botones programables de gamepad/ratón
OS_DETECTION_ENABLE = yes            # Habilitar detección automática del sistema operativo del host
SEQUENCER_ENABLE = no               # Deshabilitar secuenciador musical paso a paso
RAW_ENABLE = no                     # Deshabilitar transferencia de datos HID crudos (Raw HID)
IOS_DEVICE_ENABLE = no              # Deshabilitar modo de bajo consumo para compatibilidad con iOS
SPLIT_KEYBOARD = no                 # Deshabilitar soporte para teclados divididos

# ------------------------------------------------------------------------------
# 7. Otros Ajustes y Protocolos
# ------------------------------------------------------------------------------
ADHLNS_ENABLE = no                  # Deshabilitar optimizaciones específicas ADHLNS
BOOTMAGIC_ENABLE = no               # Deshabilitamos soporte completo de Bootmagic
DEFERRED_EXEC_ENABLE = no           # Deshabilitar ejecución diferida de funciones (timers)
CRC_ENABLE = no                     # Deshabilitar cálculo CRC de hardware/software no crítico
EXTRAKEY_ENABLE = no                # Habilitar teclas multimedia, control de volumen y encendido
MOUSEKEY_ENABLE = no                # Habilitar control de cursor de ratón mediante teclado
NKRO_ENABLE = yes                   # Habilitar N-Key Rollover (pulsación simultánea ilimitada de teclas)
WATCHDOG_ENABLE = no                # Deshabilitar temporizador Guardián (Watchdog) para reinicio del sistema
NO_USB_STARTUP_CHECK = no           # Realizar comprobación del estado del bus USB al arrancar
USB_WAIT_FOR_ENUMERATION = no       # No forzar espera a la enumeración USB completa al arrancar
USB_HID_ENABLE = no                 # Deshabilitar soporte para helpers de protocolo USB HID adicionales
NO_SUSPEND_POWER_DOWN = no          # Permitir entrar en bajo consumo en suspensión USB
JOYSTICK_SHARED_EP = no             # No compartir endpoint USB para el Joystick
DIGITIZER_SHARED_EP = no            # No compartir endpoint USB para el Digitalizador
KEYBOARD_SHARED_EP = no             # No compartir endpoint USB del teclado
MOUSE_SHARED_EP = no                # No compartir endpoint USB del ratón
