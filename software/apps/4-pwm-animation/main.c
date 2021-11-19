// MAIN

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "led_matrix.h"
#include "led_text.h"

int main(void) {

  printf("Board started!\n");

  // initialize PWM
  pwm_init();

  // start pwm
  play_pwm();

  // Set display text
  set_text(0, "Hello, welcome to CE346!", 24);

  // Start animating text
  play_text(0, 20, 20, 20, 0.05);

  // Set brightness
  set_brightness_text(0.5);

  // Delay for 10 seconds
  nrf_delay_ms(10000);

  // End animation
  stop_text();

  while (true) {
    
    nrf_delay_ms(1000);

  }

}
