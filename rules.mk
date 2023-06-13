CAPS_WORD_ENABLE = yes
CUSTOM_MATRIX = lite
WS2812_DRIVER = vendor
RGBLIGHT_ENABLE = no
DEFERRED_EXEC_ENABLE = yes
#SRC += encoder.c
SRC += matrix.c

LTO_ENABLE = yes

# JOYSTICK
JOYSTICK_ENABLE = yes
JOYSTICK_DRIVER = analog
# Add analog functionality to the source tree
SRC += analog.c

# ENCODER
# ENCODER_MAP_ENABLE = yes
ENCODER_ENABLE = yes

# OLED
OLED_ENABLE = yes
OLED_DRIVER = SSD1306

DEFAULT_FOLDER = plusmini

