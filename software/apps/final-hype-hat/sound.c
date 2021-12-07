
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_pwm.h"
#include "nrfx_saadc.h"

#include "microbit_v2.h"

#include "sound.h"


// Analog input
#define ANALOG_MIC_IN NRF_SAADC_INPUT_AIN3

// ADC channel configurations
#define ADC_MIC_CHANNEL 0
#define ADC_MAX_COUNTS (16384)

// Timer configuration
#define TIMER_TICKS (16000000/SAMPLING_FREQUENCY)

// PWM configuration
static const nrfx_pwm_t PWM_INST = NRFX_PWM_INSTANCE(0);

// Sample data configurations
#define SAMPLING_FREQUENCY 16000 // 16 kHz sampling rate
#define BUFFER_SIZE 16000 // 1 second of data
uint16_t samples[BUFFER_SIZE] = {0}; // stores ADC samples and PWM duty cycle values
uint32_t amplitudeAverage = 0;

volatile bool samples_complete = false; // flag for blocking while sampling


void TIMER4_IRQHandler(void) {

  // Clear the event
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;

  // Set the next timer based on the previous (avoid drift)
  NRF_TIMER4->CC[0] = NRF_TIMER4->CC[0] + TIMER_TICKS;

  // Sample the ADC (captures data over DMA, non-blocking)
  nrfx_saadc_sample();
}

static void saadc_event_callback(nrfx_saadc_evt_t const* event) {
  if (event->type == NRFX_SAADC_EVT_DONE) {

    // Done sampling, stop the timer
    NRF_TIMER4->CC[0] = 0;
    NRF_TIMER4->TASKS_STOP = 1;

    printf("ADC sampling complete (%d samples)\n", event->data.done.size);

    // adjust the data here before returning
    // Warning: don't try to print all the ADC samples without adding an nRF
    //  delay every few samples. It really messes up the system. Something
    //  definitely breaks and it needs to be re-programmed to start again

    // determine the average of the samples
    uint32_t average = 0;
    for (int i=0; i<BUFFER_SIZE; i++) {
      average += (uint16_t)samples[i];
    }

    average = average/BUFFER_SIZE;

    uint32_t amplitudeTotal = 0;

    // scale each sample based on the average value and recenter around 50%
    for (int i=0; i<BUFFER_SIZE; i++) {
      // scaling determined experimentally by Branden Ghena
      samples[i] = (((int32_t)samples[i] - average) * 10) + (ADC_MAX_COUNTS/2);

      amplitudeTotal += samples[i];
    }

      amplitudeAverage = amplitudeTotal/BUFFER_SIZE;
      // printf("%d\n", amplitudeAverage);
    // Signal completion
    samples_complete = true;

  } else {
    printf("Got some other SAADC event?!\n");
  }
}

static void gpio_init(void) {
  // Initialize pins
  // Microphone pin MUST be high drive
  nrf_gpio_pin_dir_set(LED_MIC, NRF_GPIO_PIN_DIR_OUTPUT);
  nrf_gpio_cfg(LED_MIC, NRF_GPIO_PIN_DIR_OUTPUT, NRF_GPIO_PIN_INPUT_DISCONNECT,
      NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_S0H1, NRF_GPIO_PIN_NOSENSE);

}

static void adc_init(void) {
  // Initialize the SAADC
  nrfx_saadc_config_t saadc_config = {
    .resolution = NRF_SAADC_RESOLUTION_14BIT,
    .oversample = NRF_SAADC_OVERSAMPLE_DISABLED,
    .interrupt_priority = 1, // should be higher than timer
    .low_power_mode = false,
  };
  nrfx_saadc_init(&saadc_config, saadc_event_callback);

  // Initialize the microphone ADC channel
  // small signal sampled quickly, max out gain and minimize acquisition time
  nrf_saadc_channel_config_t mic_channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(ANALOG_MIC_IN);
  mic_channel_config.gain = NRF_SAADC_GAIN4;
  mic_channel_config.acq_time = NRF_SAADC_ACQTIME_3US;
  nrfx_saadc_channel_init(ADC_MIC_CHANNEL, &mic_channel_config);
}

static void timer_init(void) {
   // Set to 32 bit timer
  NRF_TIMER4->BITMODE = 3;

  // Set to 16 MHz clock
  NRF_TIMER4->PRESCALER = 0;

  // Enable interrupts (not the 0th bit!)
  NRF_TIMER4->CC[0] = 0;
  NRF_TIMER4->INTENSET = 1 << TIMER_INTENSET_COMPARE0_Pos;

  // Enable interrupts in the NVIC
  NVIC_ClearPendingIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 0); // lowest priority
  NVIC_EnableIRQ(TIMER4_IRQn);
}

static void sample_microphone(void) {
  // save a flag for blocking on later
  samples_complete = false;

  // prepare the sample the ADC
  nrfx_saadc_buffer_convert((int16_t*)samples, BUFFER_SIZE);

  // clear and start timer, set interrupt
  NRF_TIMER4->TASKS_CLEAR = 1;
  NRF_TIMER4->TASKS_START = 1;
  NRF_TIMER4->CC[0] = TIMER_TICKS; // 16 kHZ
}

static int get_sound(void) {
  
  int min = 8000;
  int max = 35000;
  int scaled;
  
  if (amplitudeAverage < min) {
    return 0;
  }
  else if (amplitudeAverage > max) {
    return 100;
  }
  
  scaled = ((amplitudeAverage-min)*100/(max-min)); //Scaled valued of sound amplitude from 0-100
  return scaled;
  
}

void sound_init(void){
 // Initialize GPIO
  gpio_init();

  // Initialize ADC
  adc_init();

  // Initialize timer
  timer_init();

}


int read_sound(void){

  // Enable microphone
  nrf_gpio_pin_set(LED_MIC);

  // Sample audio from the microphone
  sample_microphone();

  // Delay until sampling the ADC is complete
  while (!samples_complete) {
    nrf_delay_ms(100);
  }


  // Disable microphone
  nrf_gpio_pin_clear(LED_MIC);

  int result = get_sound();

  printf("\nAmplitude Average: %d\n", amplitudeAverage);
  printf("Sound Sensor: %d\n\n", result);

  return result;
}

