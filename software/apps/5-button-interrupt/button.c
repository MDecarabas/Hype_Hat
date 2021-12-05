#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"

#include "microbit_v2.h"
#include "button.h"

#define BTN  BTN_A


// Initialize all necessary drivers/modules to enable interrupt event
void button_init() {

  // Set pin as input and clear it 
  nrf_gpio_cfg(BTN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT, 
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_pin_clear(BTN);

  // Initialize and enable GPIOTE module
  nrfx_gpiote_init();
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true); // high-accuracy mode
  nrfx_gpiote_in_init(BTN, &in_config, gpio_handler);
  nrfx_gpiote_in_event_enable(BTN, true); // enable interrupts

}


// 
void button_set_callback(void (*f)) {
  
}
