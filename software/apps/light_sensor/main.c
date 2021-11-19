#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_twi_mngr.h"
#include "nrfx_pwm.h"

#include "microbit_v2.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);
APP_TIMER_DEF(my_timer_1);

//////////////////////////////////////////////////////////////////////////////////////////

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

// Helper function to perform a 1-byte I2C read of a given register
//
// i2c_addr - address of the device to read from
// reg_addr - address of the register within the device to read
//
// returns 16-bit read value
static uint16_t i2c_i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint16_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    // .transWrite = 
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),

    // .transRead = 
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 2, 0)
    //TODO: implement me
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);

  return rx_buf;
}

// Helper function to perform a 1-byte I2C write of a given register
//
// i2c_addr - address of the device to write to
// reg_addr - address of the register within the device to write
static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  //TODO: implement me
    uint8_t writeAdrs [2] = {reg_addr, data};
    nrf_twi_mngr_transfer_t const write_transfer[] = {
    
    NRF_TWI_MNGR_WRITE(i2c_addr, writeAdrs, 2, 0)
    };

  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);

  //Note: there should only be a single two-byte transfer to be performed
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void lsm303agr_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  // ---Initialize light sensor---

  // Enable light sensor
  i2c_reg_write(0x5C, 0b00100011, 0x10);

  // Wait 180 ms
}

// check the light measure
uint16_t lsm303agr_read_light(void) {
  uint16_t data_read = i2c_reg_read(0x5C, 0b00100011);
  print("%u", data_read);
  return data_read;
}

int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = I2C_SCL;
  i2c_config.sda = I2C_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  i2c_config.interrupt_priority = 0;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Loop forever
  while (1) {
    // Don't put any code in here. Instead put periodic code in a callback using a timer.
    nrf_delay_ms(1000);
  }
}

