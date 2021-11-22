#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "nrf_delay.h"
#include "light.h"

//////////////////////////////////////////////////////////////////////////////////////////

// Pointer to an initialized I2C instance to use for transactions
static const nrf_twi_mngr_t* i2c_manager = NULL;

// Helper function to perform a 1-byte I2C read of a given register
//
// i2c_addr - address of the device to read from
// reg_addr - address of the register within the device to read
//
// returns 16-bit read value
static uint16_t i2c_reg_read(uint8_t i2c_addr) {
  uint16_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    // .transRead = 
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 2, 0)
    //TODO: implement me
  };
  nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 1, NULL);

  return rx_buf;
}

// Helper function to perform a 1-byte I2C write of a given register
//
// i2c_addr - address of the device to write to
// reg_addr - address of the register within the device to write
static void i2c_reg_write(uint8_t i2c_addr, uint8_t data) {
  //TODO: implement me
    uint8_t writeAdrs[] = {data};
    nrf_twi_mngr_transfer_t const write_transfer[] = {
    
    NRF_TWI_MNGR_WRITE(i2c_addr, writeAdrs, 1, 0)
    };

  nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);

  //Note: there should only be a single two-byte transfer to be performed
}

// Initialize and configure the LSM303AGR accelerometer/magnetometer
//
// i2c - pointer to already initialized and enabled twim instance
void i2c_light_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

  // ---Initialize light sensor---

  // Enable light sensor
  // power on
  // i2c_reg_write(0x5C, 0x23, 0x01);
  i2c_reg_write(0x5C, 0x10);
  nrf_delay_ms(200);
}

// check the light measure
// Scale to range of 0-100
uint16_t read_light(void) {
  uint16_t data_read = i2c_reg_read(0x5C);
  printf("%u\n", data_read);
  return data_read;
}