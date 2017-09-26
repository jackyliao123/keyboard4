#ifndef util_h
#define util_h

#include <inttypes.h>

void util_init(void);
uint32_t util_micros(void);
void util_key_poll(void);

void util_key_update(uint8_t key, uint8_t state);

uint8_t _key_state[4];
uint32_t _key_last_change[4];

#endif
