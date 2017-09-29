#include "tlc5940.h"
#include "usb.h"
#include <avr/io.h>

static uint8_t led_buf[24];

ISR(TIMER1_OVF_vect) {
	// XLAT processed, disable XLAT
	TCCR1A = _BV(COM1B1);
}

void tlc5940_update(void) {
	// Disable XLAT 
	TCCR1A = _BV(COM1B1);

	// Send data
	for(uint8_t i = 0; i < 24; ++i) {
		SPDR = led_buf[i];
		while(!(SPSR & _BV(SPIF)));
	}

	// Queue XLAT 
	TCCR1A = _BV(COM1A1) | _BV(COM1B1);
}

void tlc5940_set_rgb(uint8_t led, uint16_t r, uint16_t g, uint16_t b) {
	uint8_t ind = (led << 1) + led;
	tlc5940_set(ind, r);
	tlc5940_set(ind + 1, b);
	tlc5940_set(ind + 2, g);
}

void tlc5940_set(uint8_t led, uint16_t val) {
	TCCR1A = _BV(COM1B1);
	int ind = 23 - (led & 0xE) - (led >> 1);
	if(led & 1) {
		led_buf[ind - 1] = (led_buf[ind - 1] & 0xF) | (val << 4);
		led_buf[ind - 2] = val >> 4;
	} else {
		led_buf[ind] = val;
		led_buf[ind - 1] = (led_buf[ind - 1] & 0xF0) | ((val >> 8) & 0xF);
	}
	TCCR1A = _BV(COM1A1) | _BV(COM1B1);
}

void tlc5940_init(void) {

	// SS SCLK MOSI MISO XLAT BLANK
	DDRB |= _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(5) | _BV(6);

	// Hold BLANK high
	PORTB |= _BV(6);

	// GSCLK
	DDRC |= _BV(6);

	// Init SPI
	SPSR = _BV(SPI2X);
	SPCR = _BV(SPE) | _BV(MSTR);

	// Dot correction register
	PORTB |= _BV(3);

	// Send DC data
	for(uint8_t i = 0; i < 12; ++i) {
		SPDR = 0xFF;
		while(!(SPSR & _BV(SPIF)));
	}

	// Pulse XLAT
	PORTB |= _BV(5);
	PORTB &= ~_BV(5);

	// Grayscale register
	PORTB &= ~_BV(3);

	tlc5940_update();

	// Pulse XLAT
	PORTB |= _BV(5);
	PORTB &= ~_BV(5);

	// Init Timer1 for BLANK and XLAT
	OCR1A = 1;
	OCR1B = 2;
	ICR1 = 8192; // 2732
	TCCR1B = _BV(CS10) | _BV(WGM13);

	// Init Timer3 for GSCLK
	TCCR3A = _BV(COM3A0);
	TCCR3B = _BV(WGM32) | _BV(CS30);
	OCR3A = 1;

	// Start BLANK pulses
	TCCR1A = _BV(COM1B1);

	tlc5940_update();
}
