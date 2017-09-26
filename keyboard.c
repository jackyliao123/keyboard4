#include "usb.h"
#include "tlc5940.h"
#include "util.h"
#include <avr/eeprom.h>

uint8_t clr = 0;

uint16_t brightness[4] = {1, 16, 2047, 4095};

void util_key_update(uint8_t key, uint8_t state) {
	if(state) {
//		usb_mouse_button_down(0);
		usb_keyboard_key_down(key == 2 ? 27 : 29);
		tlc5940_set_rgb(key, 4095, 0, 4095);
	} else {
//		usb_mouse_button_up(0);
		usb_keyboard_key_up(key == 2 ? 27 : 29);
		tlc5940_set_rgb(key, 0, 0, 0);
	}
	tlc5940_update();
	
	clr = (clr + 1) % 3;
}

void usb_cdc_recv(uint8_t *data, uint8_t len) {
//	usb_cdc_send(data, len);
}

void usb_keyboard_update_led(uint8_t led) {
	printf("LED state change: %hhu\n", led);
}

int main(void) {
	// Initialize things
	usb_init();
	tlc5940_init();
	util_init();

	// Map stdout to USB serial
	FILE f = usb_cdc_stdout_create_stream();
	__iob[1] = &f;

	while(1) {
		// Poll for key state changes 
		util_key_poll();
//		tlc5940_update();
		usb_run();

	}
}

