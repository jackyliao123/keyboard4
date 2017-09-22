#include "usb.h"
#include "tlc5940.h"
#include <avr/eeprom.h>

const uint8_t KEY_MASKS[4] = {0b1000, 0b0100, 0b0010, 0b0001};

uint32_t ctr;
uint32_t key_last_change[4];
uint8_t key_state[4];

uint32_t debounce_timeout = 10000;

uint8_t clr = 0;

uint16_t brightness[4] = {1, 16, 2047, 4095};

void process_key(uint8_t key, uint8_t state) {
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
}

ISR(TIMER0_OVF_vect) {
	ctr += 128;
}

uint32_t micros(void) {
	return ctr | (TCNT0 >> 1);
}

struct test {
	int a;
};

int main(void) {
	usb_init();
	tlc5940_init();
	FILE f = usb_cdc_stdout_create_stream();
	__iob[1] = &f;
	TCCR0B |= 2;
	TIMSK0 |= 1;
	
	DDRD &= ~0xF;
	PORTD |= 0xF;

	tlc5940_set_rgb(3, 255, 255, 255);
	tlc5940_update();

	while(1) {
		uint32_t current_time = micros();
		uint8_t port_state = PIND & 0xF;
		for(uint8_t i = 0; i < 4; ++i) {
			uint8_t key_current_state = !(port_state & KEY_MASKS[i]);
			if(key_current_state != key_state[i] && current_time - key_last_change[i] > debounce_timeout) {
				key_state[i] = key_current_state;
				process_key(i, key_current_state);
				key_last_change[i] = current_time;
			}
		}

//		tlc5940_update();
		usb_run();

	}
}

