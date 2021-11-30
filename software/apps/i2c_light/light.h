// light sensor

#pragma once

#include "nrf_twi_mngr.h"

// light sensor i2c address
static const uint8_t LIGHT_ADDRESS = 0x5C;

// Function prototypes

// Initialize and configure the light sensor
//
// i2c - pointer to already initialized and enabled twim instance
void light_init(const nrf_twi_mngr_t* i2c);

// Read the light sensed
//
// Return measurement as floating point value in lux
float read_light(void);
