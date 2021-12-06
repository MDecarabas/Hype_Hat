// sound sensor header file

#include "nrf_twi_mngr.h"

// Light sensor i2c address
static const uint8_t LIGHT_ADDRESS = 0x5C;

// Read the sound sensed
int read_sound(void);

// Initialize sound sensor
void sound_init(void);
