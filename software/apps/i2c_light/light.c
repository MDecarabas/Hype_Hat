// Light driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of light

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "light.h"
#include "nrf_delay.h"

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void light_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  // ---Initialize Light Sensor---
  // set the light sensor to continuous H-Resolution Mode
  nrf_twi_mngr_transfer_t const initial_transfer[] = {
    NRF_TWI_MNGR_WRITE(LIGHT_ADDRESS, 0x10, 1, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, initial_transfer, 1, NULL);
  nrf_delay_ms(200); // needs time to update
}

// Read the external light sensor
//
// Return measurement as floating point value in lux
float read_light(void) {

  uint16_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_READ(LIGHT_ADDRESS, &rx_buf, 2, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
  float light_read = (float) rx_buf;
  printf("Light: %f\n\n", light_read);

  return light_read;
}
