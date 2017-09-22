#ifndef usb_h 
#define usb_h 

#include <avr/wdt.h>
#include <avr/power.h>

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void usb_init(void);
void usb_run(void);

bool usb_cdc_send(uint8_t *data, uint8_t len);
void usb_cdc_recv(uint8_t *data, uint8_t len);
FILE usb_cdc_stdout_create_stream(void);

void usb_keyboard_update_led(uint8_t led);
void usb_keyboard_key_down(uint8_t key);
void usb_keyboard_key_up(uint8_t key);
void usb_keyboard_modifier_down(uint8_t modifier);
void usb_keyboard_modifier_up(uint8_t modifier);

void usb_mouse_button_down(uint8_t button);
void usb_mouse_button_up(uint8_t button);
void usb_mouse_move(int16_t x, int16_t y);

#endif
