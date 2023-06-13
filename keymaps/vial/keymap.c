// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <stdio.h>
//#include "keycodes.h"
#include "joystick.h"
#include "analog.h"

static int actuation = 256; // actuation point for arrows (0-511)
bool arrows[4];

#define KC_COPY  LCTL(KC_C)
#define KC_1E LCTL(KC_V)
#define KC_CUT   LCTL(KC_X)
#define KC_UNDO  LCTL(KC_Z) 
#define KC_FIND  LCTL(KC_F) 
#define KC_REDO  LCTL(KC_Y)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┐
      * │ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ A │ S │ D │ F │ G │       │ H │ J │ K │ L │ ; │
      * ├───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┤
      * │ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │
      * └───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┘
      *           ┌───┐                   ┌───┐
      *           │GUI├───┐           ┌───┤Alt│
      *           └───┤Bsp├───┐   ┌───┤Ent├───┘
      *               └───┤   │   │   ├───┘
      *                   └───┘   └───┘
      */
    [0] = LAYOUT(
        KC_ESC,   KC_Q,   KC_W,   KC_E,  KC_R,   KC_T,   KC_Y,   KC_U,  KC_I,   KC_O,   KC_P, KC_LGUI,
        KC_LSFT,   KC_A,   KC_S,   KC_D,  KC_F,   KC_G,   KC_H,   KC_J,  KC_K,   KC_L,   KC_SCLN, KC_SPC,
        KC_LCTL,   KC_Z,   KC_X,   KC_C,  KC_V,   KC_B,   KC_N,   KC_M,  KC_COMM,KC_DOT, KC_SLSH, KC_ENT
    )
};

// init devices functions

int get_icon_start_position(int key_position) {
    if (key_position == 1) {
        return 0;
    } else {
        return (key_position - 1) * 3;
    }
}




// OLED + ENCODER PART START

#ifdef OLED_ENABLE

#define INFO_TIMEOUT 4000 

bool is_timer_active = false;
static uint16_t info_timer;

char keylog_str[24];
const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    ' ', 'E', 'B', 'T', '_', '-', '=', '[', '+', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

int flipflop = 0;

/* -----------------------------------------------------------------------------------
   functions
   ------------------------------------------------------------------------------------ */

oled_rotation_t oled_init_user(oled_rotation_t rotation);
void            housekeeping_task_user(void);
bool            oled_task_user(void);
bool            process_record_user(uint16_t keycode, keyrecord_t *record);
void            oled_render_layer_state(int i);
void            set_keylog(uint16_t keycode, keyrecord_t *record);
void            oled_render_keylog(void);
void            oled_render_logo(void);
void            start_timer(void);
void            check_timer(void);


oled_rotation_t oled_init_user(oled_rotation_t rotation)
{
    return OLED_ROTATION_180;
} // End of oled_init_user.


void oled_render_layer_state(int i) 
{
   oled_write_P(PSTR("LAYER: "), false);

   switch (i) {
      case 0:
         oled_write_ln_P(PSTR("BASE"), false);
            oled_write_ln_P(PSTR("    "), false);
               oled_write_ln_P(PSTR("    "), false);
         break;
      case 1:
         oled_write_ln_P(PSTR(" L1 "), false);
            oled_write_ln_P(PSTR("    "), false);
               oled_write_ln_P(PSTR("    "), false);
         break;
      case 2:
         oled_write_ln_P(PSTR(" L2 "), false);
            oled_write_ln_P(PSTR("    "), false);
               oled_write_ln_P(PSTR("    "), false);
         break;
   }
} // End of oled_render_layer_state.


bool encoder_update_user(uint8_t index, bool clockwise) {
    static const char PROGMEM UP_ICON[] = {0x1E,0};
    static const char PROGMEM DOWN_ICON[] = {0x1F,0};
    if (index == 0) {
        if (layer_state_is(0)) {
            if (clockwise) { 
               layer_state_set(0x00000002);
                oled_render_layer_state(1);
                oled_set_cursor(get_icon_start_position(7), 3);
                oled_write_P(PSTR(" "), false);
                oled_set_cursor(get_icon_start_position(7), 2);
                oled_write_P(UP_ICON, false);
            } else { 
                layer_state_set(0x00000004);
                oled_render_layer_state(2);
                oled_set_cursor(get_icon_start_position(7), 2);
                oled_write_P(PSTR(" "), false);
                oled_set_cursor(get_icon_start_position(7), 3);
                oled_write_P(DOWN_ICON, false);
            }
        } else if (layer_state_is(1)) {
            if (clockwise) {
                layer_state_set(0x00000004);
                oled_render_layer_state(2);
                oled_set_cursor(get_icon_start_position(7), 3);
                oled_write_P(PSTR(" "), false);
                oled_set_cursor(get_icon_start_position(7), 2);
                oled_write_P(UP_ICON, false);
            } else {
                layer_state_set(0x00000001);
                oled_render_layer_state(0);
                oled_set_cursor(get_icon_start_position(7), 2);
                oled_write_P(PSTR(" "), false);
                oled_set_cursor(get_icon_start_position(7), 3);
                oled_write_P(DOWN_ICON, false);
            }
        } else if (layer_state_is(2)) {
            if (clockwise) {
                layer_state_set(0x00000001);
                oled_render_layer_state(0);
                oled_set_cursor(get_icon_start_position(7), 3);
                oled_write_P(PSTR(" "), false);
                oled_set_cursor(get_icon_start_position(7), 2);
                oled_write_P(UP_ICON, false);
            } else {
                layer_state_set(0x00000002);
                oled_render_layer_state(1);
                oled_set_cursor(get_icon_start_position(7), 2);
                oled_write_P(PSTR(" "), false);
                oled_set_cursor(get_icon_start_position(7), 3);
                oled_write_P(DOWN_ICON, false);
            }
        }
    }
    return true;
}


void set_keylog(uint16_t keycode, keyrecord_t *record)
{
   char name = ' ';
   if(keycode < 60) { name = code_to_name[keycode]; } 
   else             { name = ' '; }

   switch (keycode) {
      case KC_ESC:
         snprintf(keylog_str, sizeof(keylog_str), "> ESCAPE <    ");
         break;
      case KC_BSPC:
         snprintf(keylog_str, sizeof(keylog_str), "> BACKSPACE < ");
         break;
      case KC_ENT:
         snprintf(keylog_str, sizeof(keylog_str), "> ENTER <     ");
         break;
      case KC_SPC:
         snprintf(keylog_str, sizeof(keylog_str), "> SPACE <     ");
         break;  
      case KC_TAB:
         snprintf(keylog_str, sizeof(keylog_str), "> TAB <        ");
         break;
      case KC_DEL:
         snprintf(keylog_str, sizeof(keylog_str), "> DEL <       ");
         break; 

      /*case TG(2):
         if(flipflop == 0)      { flipflop = 1; snprintf(keylog_str, sizeof(keylog_str), "> OTHER <           "); }
		 else if(flipflop == 1) { flipflop = 0; snprintf(keylog_str, sizeof(keylog_str), "> BASE <           "); }
         break;*/
      case KC_NO:
         snprintf(keylog_str, sizeof(keylog_str), "> XXXXX <     ");
         break;
      case KC_CALC:
         snprintf(keylog_str, sizeof(keylog_str), "> CALC <      ");
         break;

      case KC_PSLS:
         snprintf(keylog_str, sizeof(keylog_str), "> / <         ");
         break;
      case KC_PAST:
         snprintf(keylog_str, sizeof(keylog_str), "> * <         ");
         break;
      case KC_PMNS:
         snprintf(keylog_str, sizeof(keylog_str), "> - <         ");
         break;
      case KC_PPLS:
         snprintf(keylog_str, sizeof(keylog_str), "> + <         ");
         break;

      case KC_COPY:
         snprintf(keylog_str, sizeof(keylog_str), "> COPY <      ");
         break;
      case KC_PASTE:
         snprintf(keylog_str, sizeof(keylog_str), "> PASTE <     ");
         break;
      case KC_CUT:
         snprintf(keylog_str, sizeof(keylog_str), "> CUT <       ");
         break;
      case KC_UNDO:
         snprintf(keylog_str, sizeof(keylog_str), "> UNDO <      ");
         break;
      case KC_REDO:
         snprintf(keylog_str, sizeof(keylog_str), "> REDO <      ");
         break;
      case KC_FIND:
         snprintf(keylog_str, sizeof(keylog_str), "> FIND <      ");
         break;
 
      case KC_MPLY:
         snprintf(keylog_str, sizeof(keylog_str), "> PLAY/STOP < ");
         break; 
      case KC_UP:
         snprintf(keylog_str, sizeof(keylog_str), "> UP <        ");
         break;        
      case KC_DOWN:
         snprintf(keylog_str, sizeof(keylog_str), "> DOWN <      ");
         break;
      case KC_LEFT:
         snprintf(keylog_str, sizeof(keylog_str), "> LEFT <     ");
         break; 
      case KC_RIGHT:
         snprintf(keylog_str, sizeof(keylog_str), "> RIGHT <    ");
         break;

      default:
         snprintf(keylog_str, sizeof(keylog_str), "> %c <    ", name);
         break;
   } 
} // End of set_keylog.


void oled_render_keylog(void) 
{
   oled_write(keylog_str, false);
         oled_write_ln_P(PSTR("    "), false);
} // End of oled_render_keylog.


void oled_render_logo(void)
{
   // 'plusmini_logo', 128x32px
   static const char PROGMEM cici_logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
      0};

   oled_write_P(cici_logo, false);
   oled_write_ln_P(PSTR("    "), false);
} // End of oled_render_logo.


void start_timer(void)
{
   info_timer = timer_read();
   is_timer_active = true;
} // End of start_timer.


void check_timer(void)
{
   if (timer_elapsed(info_timer) > INFO_TIMEOUT) {
      oled_render_logo();
      is_timer_active = false;
   }
} // End of check_timer.


void housekeeping_task_user(void)
{
   if(is_timer_active){
      check_timer();
   }
} // End of housekeeping_task_user.


 bool oled_task_user(void) 
{ 
   oled_set_brightness(100);

   return(false);
} // End of oled_task_user.


bool process_record_user(uint16_t keycode, keyrecord_t *record)
{   
   if (record->event.pressed) { 
      if (layer_state_is(0)) { 
      oled_render_layer_state(0); set_keylog(keycode, record); oled_render_keylog();
      };
      if (layer_state_is(1)) { 
      oled_render_layer_state(1); set_keylog(keycode, record); oled_render_keylog();
      };
      if (layer_state_is(2)) { 
      oled_render_layer_state(2); set_keylog(keycode, record); oled_render_keylog();
      };
   } 
   else {
      start_timer();
   }

   return true;
} // End of process_record_user. 

#endif 

// OLED + ENCODER PART END




// ANALOG PART START

void matrix_scan_user(void) {
	  // Up
  	if (!arrows[0] && analogReadPin(B6) - 512 > actuation) {
  			arrows[0] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,0);
  			register_code16(keycode);
  	} else if (arrows[0] &&  analogReadPin(B6) - 512 < actuation) {
  			arrows[0] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,0);
  			unregister_code16(keycode);
  	}
		// Down
  	if (!arrows[1] && analogReadPin(B6) - 512 < -actuation) {
  			arrows[1] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,1);
  			register_code16(keycode);
  	}	else if (arrows[1] && analogReadPin(B6) - 512 > -actuation) {
  			arrows[1] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,1);
  			unregister_code16(keycode);
  	}
    // Left
  	if (!arrows[2] && analogReadPin(D7) - 512 > actuation) {
  			arrows[2] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,2);
  			register_code16(keycode);
  	} else if (arrows[2] &&  analogReadPin(D7) - 512 < actuation) {
  			arrows[2] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,2);
  			unregister_code16(keycode);
  	}
    // Right
  	if (!arrows[3] && analogReadPin(D7) - 512 < -actuation) {
  			arrows[3] = true;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,3);
  			register_code16(keycode);
  	} else if (arrows[3] && analogReadPin(D7) - 512 > -actuation) {
  			arrows[3] = false;
  			uint16_t keycode = dynamic_keymap_get_keycode(biton32(layer_state), 3,3);
  			unregister_code16(keycode);
  	}
}

// Joystick config
joystick_config_t joystick_axes[JOYSTICK_AXES_COUNT] = {
    [0] = JOYSTICK_AXIS_VIRTUAL(D7, 0, 512, 1023),
    [1] = JOYSTICK_AXIS_VIRTUAL(B6, 0, 512, 1023);
};

// ANALOG PART END
