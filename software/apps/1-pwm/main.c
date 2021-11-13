// PWM LED Matrix

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"

// PWM configuration
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Holds duty cycle values to trigger PWM toggle
nrf_pwm_values_common_t sequence_data[1] = {0};

// Sequence structure for configuring DMA
nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = 1,
  .repeats = 0,
  .end_delay = 0,
};

static void pwm_init(void) {
  
  // Configuration for nrfx_pwm_config_t
  nrfx_pwm_config_t pwm_config = {
    .output_pins = {EDGE_P13, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED, NRFX_PWM_PIN_NOT_USED},
    .irq_priority = 1,
    .base_clock = NRF_PWM_CLK_16MHz,
    .count_mode = NRF_PWM_MODE_UP,
    .top_value = 20,
    .load_mode = NRF_PWM_LOAD_COMMON,
    .step_mode = NRF_PWM_STEP_AUTO,
  };

  nrfx_pwm_init(&PWM_INST, &pwm_config, NULL);
}

static void play_pwm() {

  // Stop the PWM (and wait until its finished)
  nrfx_pwm_stop(&PWM_INST, true);

  // Set compare value
  // HIGH = 13
  // LOW = 6;
  sequence_data[0] =  6 + (1 << 15);

  // Start playback of the samples and loop indefinitely
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);

}

int main(void) {

  printf("Board started!\n");

  // initialize PWM
  pwm_init();

  // start pwm
  play_pwm();
}


