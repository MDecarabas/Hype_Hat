// I2C sensors app
//
// Read from I2C light sensor

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "light.h"
#include "app_timer.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);
APP_TIMER_DEF(my_timer_1);


int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = EDGE_P19;
  i2c_config.sda = EDGE_P20;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Initialize the light sensor
  light_init(&twi_mngr_instance);

  //TODO: implement me!
  app_timer_init();
  app_timer_create(&my_timer_1, APP_TIMER_MODE_REPEATED, read_light);
  app_timer_start(my_timer_1, 32768, NULL);

  // Loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in a callback using a timer.
    //nrf_delay_ms(1000);
    //read_light();
  }
}

