#include "usb.h"
#include "tlc5940.h"
#include "util.h"

#include <stdlib.h>

// Escape, C, X, Z
uint8_t key_codes[4] = {41, 6, 27, 29};

inline uint16_t sq(uint16_t in) {
	return in * in;
}

// This function must be defined, if util.h is used
void util_key_update(uint8_t key, uint8_t state) {
	srand(util_micros());
	if(state) {
		usb_keyboard_key_down(key_codes[key]);
		tlc5940_set_rgb(key, sq(random() & 63), sq(random() & 63), sq(random() & 63));
	} else {
		usb_keyboard_key_up(key_codes[key]);
		tlc5940_set_rgb(key, 0, 0, 0);
	}
	tlc5940_update();
}

// This function must be defined
void usb_cdc_recv(uint8_t *data, uint8_t len) {
//	usb_cdc_send(data, len);
}

// This function must be defined
void usb_keyboard_update_led(uint8_t led) {
//	printf("LED change: %d\n", led);
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
		usb_run();
	}
}

