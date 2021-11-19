#pragma once

#include "nrf_twi_mngr.h"

static uint16_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr);

static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data);

void i2c_light_init(const nrf_twi_mngr_t* i2c);

uint16_t read_light(void);