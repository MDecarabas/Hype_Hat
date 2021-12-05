// LED MATRIX

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"
#include "led_matrix.h"

#define SEQUENCE_SIZE 6224
#define DATA_SIZE 6144
#define LOW_BIT 6 + (1 << 15)
#define HIGH_BIT 13 + (1 << 15)
#define LED_NUM 8*32

// PWM configuration
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Holds duty cycle values to trigger PWM toggle
static nrf_pwm_values_common_t sequence_data[SEQUENCE_SIZE] = {0};


// Sequence structure for configuring DMA
static nrf_pwm_sequence_t pwm_sequence = {
  .values.p_common = sequence_data,
  .length = SEQUENCE_SIZE,
  .repeats = 0,
  .end_delay = 0,
};


// Convert LED at (row, col) to corresponding LED index
int get_led_num(int row, int col) {
  
  int num;
  if (col % 2 == 0) {
    num = (31 - (col - 1)) * 8 + (7 - (row - 1));
  } else {
    num = (31 - (col - 1)) * 8 + (row - 1);
  }
  return num;

}


// Set the RGB value for some given LED in the matrix
// led_num must be less than DATA_SIZE 
void set_data(int led_num, unsigned char r, unsigned char g, unsigned char b) {

  // Set the green values
  sequence_data[led_num*24] = !(0b10000000 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 1] = !(0b01000000 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 2] = !(0b00100000 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 3] = !(0b00010000 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 4] = !(0b00001000 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 5] = !(0b00000100 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 6] = !(0b00000010 & g)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 7] = !(0b00000001 & g)? (6 + (1 << 15)): (13 + (1 << 15));

  // Set the blue values
  sequence_data[led_num*24 + 8] = !(0b10000000 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 9] = !(0b01000000 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 10] = !(0b00100000 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 11] = !(0b00010000 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 12] = !(0b00001000 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 13] = !(0b00000100 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 14] = !(0b00000010 & r)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 15] = !(0b00000001 & r)? (6 + (1 << 15)): (13 + (1 << 15));

  // Set the red values
  sequence_data[led_num*24 + 16] = !(0b10000000 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 17] = !(0b01000000 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 18] = !(0b00100000 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 19] = !(0b00010000 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 20] = !(0b00001000 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 21] = !(0b00000100 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 22] = !(0b00000010 & b)? (6 + (1 << 15)): (13 + (1 << 15));
  sequence_data[led_num*24 + 23] = !(0b00000001 & b)? (6 + (1 << 15)): (13 + (1 << 15));

}


// Clear LED matrix
void clear_matrix() {

  // Stop the PWM (and wait until its finished)
  nrfx_pwm_stop(&PWM_INST, true);

  // Set all bits to low
  for (int i = 0; i < DATA_SIZE; i++) {
    sequence_data[i] = LOW_BIT;
  }

  // Play samples one
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, 0);

}


// Initializes the PWM driver
void pwm_init(void) {

  // Initialize sequence_data to LOW_BIT and buffer space to 1 << 15
  // This sets compare to 0 and polarity to falling edge
  for (int i = 0; i < DATA_SIZE; i++) {
    sequence_data[i] = LOW_BIT;
  }
  for (int i = DATA_SIZE; i < SEQUENCE_SIZE; i++)  {
    sequence_data[i] = 1 << 15;
  }
  
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


// Send PWM signals to LED matrix
void play_pwm() {

  // Stop the PWM (and wait until its finished)
  nrfx_pwm_stop(&PWM_INST, true);

  // Start playback of the samples and loop indefinitely
  nrfx_pwm_simple_playback(&PWM_INST, &pwm_sequence, 1, NRFX_PWM_FLAG_LOOP);

}


void stop_pwm() {

  // Stop the PWM (and wait until its finished)
  nrfx_pwm_stop(&PWM_INST, true);

}


// Print LEDs being lit up on the matrix
void debug_matrix() {

  // Declare variables needed to print the matrix
  char led_row[33];
  char curr;
  int led_index;

  // Set end character to 0 to indication string termination
  led_row[32] = 0;

  printf("Start of Matrix\n");
  // Loop through matrix to determine which LED is on
  for (int row = 1; row <= 8; row++) {
    for (int col = 1; col <= 32; col++) {
      led_index = get_led_num(row, col);
      curr = '.';
      for (int offset = 0; offset < 24; offset++) {
        if (sequence_data[led_index * 24 + offset] != LOW_BIT) {
          curr = 'O';
          break;
        }
      }
      led_row[col-1] = curr;
    }
    printf("%s\n", led_row);
  }
  printf("End of Matrix\n\n");

}

