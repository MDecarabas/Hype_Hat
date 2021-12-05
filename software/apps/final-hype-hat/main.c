// MAIN

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrfx_timer.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "led_matrix.h"
#include "led_text.h"
#include "light.h"


// Change text on button pressed
static void gpio_handler_btn_A(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  cycle_text();
}


// Change color based on microphone reading
static void gpio_handler_btn_B(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  change_color(30, 30, 30);
}


// Enable button interrupts
void enable_button_interrupt() {
  
  nrfx_gpiote_init();
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true); // high-accuracy mode

  // Interrupt event for button A
  nrf_gpio_cfg(BTN_A, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT, 
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_pin_clear(BTN_A);
  nrfx_gpiote_in_init(BTN_A, &in_config, gpio_handler_btn_A);
  nrfx_gpiote_in_event_enable(BTN_A, true); // enable interrupts

  // Interrupt event for button B
  nrf_gpio_cfg(BTN_B, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT, 
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_pin_clear(BTN_B);
  nrfx_gpiote_in_init(BTN_B, &in_config, gpio_handler_btn_B);
  nrfx_gpiote_in_event_enable(BTN_B, true); // enable interrupts
}


void init() {

  // Initalize light sensor and i2c peripheral
  light_sensor_init();

  // Initialize PWM driver with configuration specific for the LED matrix
  pwm_init();

  // Enable button interrupts
  enable_button_interrupt();

}


int main(void) {

  printf("Board started!\n");

  // Initialize light sensor,  LED matrix (along with i2c peripheral and PWM driver), and button interrupt
  init();

  //TODO: SET PRESET TEXTS
  set_text(0, "Hello, welcome to CE346!", 24, 60, 0, 0);
  set_text(1, "HYPE HAT!", 9, 0, 60, 0);
  set_text(2, "Boom Boom Pow", 13, 0, 0, 60);

  // Start animating text
  play_text(1, 0.05);

  while (true) {
    int brightness = read_light();
    if (brightness < 5) {
      set_brightness_text(5);
    }
    else {
      set_brightness_text(brightness);
    }
    nrf_delay_ms(100);
    printf("%f\n", read_light());
  }

}
