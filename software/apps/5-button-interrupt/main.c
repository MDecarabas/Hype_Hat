// MAIN

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrfx_timer.h"

#include "microbit_v2.h"

#define BTN  BTN_A

static void gpio_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
  //TODO: implement handler
  printf("Button pressed\n");
}

int main(void) {

  printf("Board started!\n");
  nrf_gpio_cfg(BTN, NRF_GPIO_PIN_DIR_INPUT, NRF_GPIO_PIN_INPUT_CONNECT, 
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0S1, NRF_GPIO_PIN_NOSENSE);
  nrf_gpio_pin_clear(BTN);
  nrfx_gpiote_init();
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(true); // high-accuracy mode
  nrfx_gpiote_in_init(BTN, &in_config, gpio_handler);
  nrfx_gpiote_in_event_enable(BTN, true); // enable interrupts

  while (true) {
    nrf_delay_ms(1000);
    //printf("%lu", nrf_gpio_pin_read(BTN));
  }

}
