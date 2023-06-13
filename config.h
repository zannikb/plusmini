// Copyright 2023 zannikb (@zannikb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

//#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
//#define WS2812_PIO_USE_PIO1                      // Force the usage of PIO1 peripheral, by default the WS2812 implementation uses the PIO0 peripheral
//#define WS2812_TRST_US 80
//#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB
//#define RGB_MATRIX_DEFAULT_VAL 32


/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

// Pick good defaults for enabling homerow modifiers
#define TAPPING_TERM 230
// #define PERMISSIVE_HOLD
//#define IGNORE_MOD_TAP_INTERRUPT
//#define TAPPING_FORCE_HOLD
//#define RETRO_TAPPING

 //#define WS2812_DI_PIN GP16               // The pin connected to the data pin of the LEDs
 //#define RGBLED_NUM 1                     // The number of LEDs connected


#define MAX_DEFERRED_EXECUTORS 32

//#define VIAL_COMBO_ENTRIES 4
//#define VIAL_KEY_OVERRIDE_ENTRIES 4
//#define DYNAMIC_KEYMAP_LAYER_COUNT 2

/* oled custom font */
//#define OLED_FONT_H "glcdfont.c"
#define OLED_FONT_H "keyboards/plusmini/lib/glcdfont.c"

/* joystick parameters */
// Max 32
#define JOYSTICK_BUTTON_COUNT 0
// Max 6: X, Y, Z, Rx, Ry, Rz
#define JOYSTICK_AXES_COUNT 2

/* Use 1000hz polling */
#define USB_POLLING_INTERVAL_MS 1