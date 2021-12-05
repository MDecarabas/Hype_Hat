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


int main(void) {
  printf("Board started!\n");

  light_sensor_init();

  // Loop forever
  while (1) {
    printf("%f\n", read_light());
    nrf_delay_ms(1000);
  }
}

