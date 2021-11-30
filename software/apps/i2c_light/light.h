// light sensor

#pragma once

#include "nrf_twi_mngr.h"

// light sensor i2c address
static const uint8_t LIGHT_ADDRESS = 0x5C;

// // Register definitions for accelerometer
// typedef enum {
//   LSM303AGR_ACC_STATUS_REG_AUX = 0X07,
//   LSM303AGR_ACC_TEMP_L = 0x0C,
//   LSM303AGR_ACC_TEMP_H = 0x0D,
//   LSM303AGR_ACC_INT_COUNTER_REG = 0X0E,
//   LSM303AGR_ACC_WHO_AM_I_REG = 0X0F,
//   LSM303AGR_ACC_TEMP_CFG_REG = 0X1F,
//   LSM303AGR_ACC_CTRL_REG1 = 0X20,
//   LSM303AGR_ACC_CTRL_REG2 = 0X21,
//   LSM303AGR_ACC_CTRL_REG3 = 0X22,
//   LSM303AGR_ACC_CTRL_REG4 = 0X23,
//   LSM303AGR_ACC_CTRL_REG5 = 0X24,
//   LSM303AGR_ACC_CTRL_REG6 = 0X25,
//   LSM303AGR_ACC_REFERENCE = 0X26,
//   LSM303AGR_ACC_STATUS_REG = 0X27,
//   LSM303AGR_ACC_OUT_X_L = 0X28,
//   LSM303AGR_ACC_OUT_X_H = 0X29,
//   LSM303AGR_ACC_OUT_Y_L = 0X2A,
//   LSM303AGR_ACC_OUT_Y_H = 0X2B,
//   LSM303AGR_ACC_OUT_Z_L = 0X2C,
//   LSM303AGR_ACC_OUT_Z_H = 0X2D,
//   LSM303AGR_ACC_FIFO_CTRL_REG = 0X2E,
//   LSM303AGR_ACC_FIFO_SRC_REG = 0X2F,
//   LSM303AGR_ACC_INT1_CFG = 0X30,
//   LSM303AGR_ACC_INT1_SRC = 0X31,
//   LSM303AGR_ACC_INT1_THS = 0X32,
//   LSM303AGR_ACC_INT1_DURATION = 0X33,
//   LSM303AGR_ACC_INT2_CFG = 0X34,
//   LSM303AGR_ACC_INT2_SRC = 0X35,
//   LSM303AGR_ACC_INT2_THS = 0X36,
//   LSM303AGR_ACC_INT2_DURATION = 0X37,
//   LSM303AGR_ACC_CLICK_CFG = 0X38,
//   LSM303AGR_ACC_CLICK_SRC = 0X39,
//   LSM303AGR_ACC_CLICK_THS = 0X3A,
//   LSM303AGR_ACC_TIME_LIMIT = 0X3B,
//   LSM303AGR_ACC_TIME_LATENCY = 0X3C,
//   LSM303AGR_ACC_TIME_WINDOW = 0X3D,
// } lsm303agr_acc_reg_t;

// Function prototypes

// Initialize and configure the light sensor
//
// i2c - pointer to already initialized and enabled twim instance
void light_init(const nrf_twi_mngr_t* i2c);

// Read the light sensed
//
// Return measurement as floating point value in lux
float read_light(void);
