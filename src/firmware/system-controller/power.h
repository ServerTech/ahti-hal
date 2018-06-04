/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * This file is a part of the AHTI hardware abstraction layer (ahti-hal).
 *
 * \file power.h
 * \author Shreyas Vinod <shreyas@shreyasvinod.xyz>
 * \date 14 Apr 2018
 *
 * \brief Power controller initialisation and management wrappers.
 */

#ifndef POWER_H_
#define POWER_H_

/**
 * \defgroup power Power Control
 * \brief Power controller initialisation and management wrappers.
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum POWER_STATUS
 * \brief Power status enumerations.
 */
typedef enum POWER_STATUS
{
  POWER_OK                 = 0x00,
  POWER_OFF                = 0x01,
  POWER_INIT               = 0x02,
  POWER_VIN_FAULT          = 0x03,
  POWER_VOUT_FAULT         = 0x04,
  POWER_OVERLIMIT          = 0x05,
  POWER_OVERTEMP           = 0x06,
  POWER_SETPOINT           = 0x07,
  POWER_PMBUS              = 0x08,

  POWER_EXP_OUT_OF_RANGE   = 0x21,
  POWER_VOUT_OUT_OF_RANGE  = 0x22,
  POWER_VIN_OUT_OF_RANGE   = 0x23,
  POWER_IOUT_OUT_OF_RANGE  = 0x24,
  POWER_POUT_OUT_OF_RANGE  = 0x25,
  POWER_UNKNOWN            = 0x39
} Power_status;

typedef struct Power_t
{
  Power_status status;             /**< power status */
  struct i2c_master_module* pmbus; /**< SERCOM instance I2C master descriptor */
  uint8_t module_count;            /**< number of power converters installed */
  bool* power_state;               /**< power state of each individual module */
  uint8_t* module_addr;            /**< SMBus addresses of each module */
  uint16_t max_power;              /**< maximum power output */
} Power_t;

struct Power_
{

};

extern const struct Power_ Power;

/**
 * \} end of power
 */

#ifdef __cplusplus
}
#endif

#endif /* POWER_H_ */
