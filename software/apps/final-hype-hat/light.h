// light sensor

#pragma once

#include "nrf_twi_mngr.h"

// Light sensor i2c address
static const uint8_t LIGHT_ADDRESS = 0x5C;

// Read the light sensed
float read_light(void);

// Initialize light sensor (I2C peripheral and the sensor)
void light_sensor_init();
