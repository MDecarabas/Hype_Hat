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
  set_text(1, "HYPE HAT!", 9);
  set_text(2, "Boom Boom Pow", 13);

  // Start animating text
  play_text(2, 40, 40, 40, 0.05);

  // Set brightness sequence
  float brightness_settings[] = {1, 0.7, 0.45, 0.3, 0.2};
  for (int i = 0; i < 5; i++) {

    // Set brightness
    set_brightness_text(0.2 * brightness_settings[i]);

    // Delay for 2 seconds
    nrf_delay_ms(2000);

  }

  // End animation
  stop_text();

  while (true) {
    
    nrf_delay_ms(1000);

  }

}
