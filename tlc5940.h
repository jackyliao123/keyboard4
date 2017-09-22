#ifndef tlc5940_h
#define tlc5940_h

#include <stdint.h>

void tlc5940_init(void);
void tlc5940_set(uint8_t led, uint16_t v);
void tlc5940_set_rgb(uint8_t led, uint16_t r, uint16_t g, uint16_t b);
void tlc5940_update(void);

#endif
