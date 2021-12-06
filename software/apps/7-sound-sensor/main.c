#include "sound.h"
#include "nrf_delay.h"

int main(void) {
  
  printf("Board started!\n");

  sound_init();  //Initializes all neccesary function to read sound

  while (1) {
    printf("Read Sound: %d\n", read_sound()); //Returns values 0-100 according to amplitude of sound
    nrf_delay_ms(1000);
  } 
}
