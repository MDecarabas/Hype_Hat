// MAIN

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "led_matrix.h"
#include "led_animate.h"

int main(void) {

  printf("Board started!\n");

  // initialize PWM
  pwm_init();

  // start pwm
  play_pwm();

  // Set display text
  set_text(0, "Boom", 4);

  // Start animating text
  play_text(0, 20, 20, 20);

  while (true) {
    
    nrf_delay_ms(1000);

  }

}
