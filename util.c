#include "util.h"
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static const uint8_t KEY_MASKS[4] = {0b1000, 0b0100, 0b0010, 0b0001};

static uint32_t ctr = 0;

ISR(TIMER0_OVF_vect) {
	ctr += 128;
}

void util_init(void) {
	TCCR0B |= _BV(CS01);
	TIMSK0 |= _BV(TOIE0);
	
	DDRD &= ~0xF;
	PORTD |= 0xF;
}

uint32_t util_micros(void) {
	return ctr | (TCNT0 >> 1);
}

void util_key_poll(void) {
	uint32_t current_time = util_micros();
	uint8_t port_state = PIND & 0xF;
	for(uint8_t i = 0; i < 4; ++i) {
		uint8_t key_current_state = !(port_state & KEY_MASKS[i]);
		if(key_current_state != _key_state[i] && current_time - _key_last_change[i] > DEBOUNCE_TIMEOUT) {
			_key_state[i] = key_current_state;
			util_key_update(i, key_current_state);
			_key_last_change[i] = current_time;
		}
	}
}
