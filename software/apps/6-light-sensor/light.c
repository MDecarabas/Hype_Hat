// Light driver for Microbit_v2
//
// Initializes sensor and communicates over I2C
// Capable of light

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "nrf_delay.h"

#include "microbit_v2.h"
#include "light.h"

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);


// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;


// i2c - pointer to already initialized and enabled twim instance
static void light_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  // ---Initialize Light Sensor---
  // set the light sensor to continuous H-Resolution Mode
  uint8_t init = 0x10;
  nrf_twi_mngr_transfer_t const initial_transfer[] = {
    NRF_TWI_MNGR_WRITE(LIGHT_ADDRESS, &init, 1, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, initial_transfer, 2, NULL);
  nrf_delay_ms(200); // needs time to update
}


// Initialize I2C peripheral and driver
// and then initialize light sensor
void light_sensor_init() {
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = EDGE_P19;
  i2c_config.sda = EDGE_P20;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  
  // Initialize the light sensor
  light_init(&twi_mngr_instance);
}


// Read the external light sensor
// Return measurement as floating point value in lux then normalize from 0-100
float read_light(void) {

  uint16_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_READ(LIGHT_ADDRESS, &rx_buf, 2, 0)
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);
  float light_read = (float) rx_buf;
  printf("Light: %f lux \n", light_read);

  // Normalize (Max value is ~64k, standard overhead light ~22k)
  light_read = light_read*100.0/66000.0;
  
  printf("Normalized: %f\n\n", light_read);

  return light_read;
}
