/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * \file smbus.h
 * \author Shreyas Vinod <shreyas@shreyasvinod.xyz>
 * \date 1 May 2018
 *
 * \brief SMBus abstraction layer for Atmel SAMD20's Application Software Framework (ASF).
 *
 * This is an abstraction layer for an SMBus master using Atmel's ASF, providing a full set of
 * I2C hardware communication functions.
 *
 * \note Requires the correct modules be set up in ASF (I2C Master and System).
 *
 * \warning These I2C operations are BLOCKING!
 */

#ifndef SMBUS_H_
#define SMBUS_H_

#include <asf.h>

/**
 * \defgroup atmel_samd20_smbus_master_blocking Atmel SAMD20 SMBus Master Abstraction (blocking)
 * \brief Atmel SAMD20 SMBus Master Abstraction (blocking)
 * \{
 */

/**
 * \def MAX_RETRIES
 * \brief Number of times to retry for I2C packet read/writes.
 */
#define MAX_RETRIES 10

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct SMBus_
 * \brief Abstraction for SMBus Master ASF internal functions.
 * \note Internal structure intended for proper use of namespaces in C.
 * \note Functions are accessed using SMBus.foo(..)
 * \note i2c_master_instance must have been properly initialised beforehand using the
 *       appropriate ASF functions.
 */

struct SMBus_
{
  enum status_code (*configure)( struct i2c_master_module *const i2c_master_instance,
                                 Sercom *const hw, uint32_t pinmux_sda, uint32_t pinmux_scl,
                                 uint32_t i2c_speed_khz );

  enum status_code (*writeBlock)( struct i2c_master_module *const i2c_master_instance, uint8_t device_address,
                                  uint8_t* data, uint32_t count );
  enum status_code (*readBlock)( struct i2c_master_module *const i2c_master_instance, uint8_t device_address,
                                 uint8_t *data, uint32_t count );

  enum status_code (*writeByte)( struct i2c_master_module *const i2c_master_instance, uint8_t device_address,
                                 uint8_t data );
  enum status_code (*readByte)( struct i2c_master_module *const i2c_master_instance, uint8_t device_address,
                                uint8_t* data );
  enum status_code (*writeWord)( struct i2c_master_module *const i2c_master_instance,
                                uint8_t device_address, uint16_t data );
  enum status_code (*readWord)( struct i2c_master_module *const i2c_master_instance,
                                uint8_t device_address, uint16_t* data );

  enum status_code (*writeByteData)( struct i2c_master_module *const i2c_master_instance,
                                     uint8_t device_address, uint8_t cmd, uint8_t data );
  enum status_code (*readByteData)( struct i2c_master_module *const i2c_master_instance,
                                    uint8_t device_address, uint8_t cmd, uint8_t* data );
  enum status_code (*writeWordData)( struct i2c_master_module *const i2c_master_instance,
                                     uint8_t device_address, uint8_t cmd, uint16_t data );
  enum status_code (*readWordData)( struct i2c_master_module *const i2c_master_instance,
                                    uint8_t device_address, uint8_t cmd, uint16_t* data );
};

extern const struct SMBus_ SMBus;

/**
 * \} end of atmel_samd20_smbus_master_blocking
 */

#ifdef __cplusplus
}
#endif

#endif // SMBUS_H_
