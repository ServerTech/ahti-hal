/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * \file smbus.c
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

#include <asf.h>

#include "smbus.h"

/**
 * \defgroup atmel_samd20_smbus_master_blocking Atmel SAMD20 SMBus Master Abstraction (blocking)
 * \brief Atmel SAMD20 SMBus Master Abstraction (blocking)
 * \{
 */

/**
 * \brief Configures the I2C master instance provided by initialising the SERCOM module
 *        correctly. Also takes care of pin multiplexing.
 *
 * Internally uses the ASF function i2c_master_init(..).
 *
 * \param [in,out] i2c_master_instance pointer to i2c_master_module instance in your main
 *                                     application
 * \param [in] hw SERCOM module to initialise I2C master on
 * \param [in] pinmux_sda SDA pin multiplexing settings
 * \param [in] pinmux_scl SCL pin multiplexing settings
 * \param [in] i2c_speed_khz I2C speed to use (in kHz - 100 or 400)
 *
 * \return status_code enumeration defined in ASF status code abstractions; returns
 *                     exactly what i2c_master_init(..) returns
 *
 */

enum status_code smbus_configure( struct i2c_master_module *const i2c_master_instance,
                                  Sercom *const hw, uint32_t pinmux_sda, uint32_t pinmux_scl,
                                  uint32_t i2c_speed_khz )
{
  enum status_code status = STATUS_OK;

  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults( &config_i2c_master );

  config_i2c_master.baud_rate   = i2c_speed_khz;
  config_i2c_master.pinmux_pad0 = pinmux_sda;
  config_i2c_master.pinmux_pad1 = pinmux_scl;

  status = i2c_master_init( i2c_master_instance, hw, &config_i2c_master );
  i2c_master_enable( i2c_master_instance );

  return status;
}

/**
 * \brief Write an arbitrary number of bytes to the specified I2C device address.
 *
 * Internally uses the ASF function i2c_master_write_packet_wait(..)
 *
 * \param [in] i2c_master_instance pointer to i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] data data byte array
 * \param [in] count array length
 *
 * \return status_code enumeration defined in ASF status code abstractions; returns
 *                     exactly what i2c_master_write_packet_wait(..) returns
 */

enum status_code smbus_writeBlock( struct i2c_master_module *const i2c_master_instance,
                                   uint8_t device_address, uint8_t* data, uint32_t count )
{
  enum status_code status = STATUS_OK;
  uint16_t timeout = 0;

  struct i2c_master_packet packet =
  {
    .address         = device_address,
    .data_length     = count,
    .data            = data,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };

  while ( ( status = i2c_master_write_packet_wait( i2c_master_instance, &packet ) ) != STATUS_OK )
  {
    if ( timeout++ >= MAX_RETRIES )
      break;
  }

  return status;
}

/**
 * \brief Read an arbitrary number of bytes from the specified I2C device address.
 *
 * Internally uses the ASF function i2c_master_read_packet_wait(..)
 *
 * \param [in] i2c_master_instance pointer to i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [out] data data byte array
 * \param [in] count number of bytes to read from slave
 *
 * \return status_code enumeration defined in ASF status code abstractions; returns
 *                     exactly what i2c_master_read_packet_wait(..) returns
 */

enum status_code smbus_readBlock( struct i2c_master_module *const i2c_master_instance,
                                  uint8_t device_address, uint8_t *data, uint32_t count )
{
  enum status_code status = STATUS_OK;
  uint16_t timeout = 0;

  struct i2c_master_packet packet =
  {
    .address         = device_address,
    .data_length     = count,
    .data            = data,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };

  while ( ( status = i2c_master_read_packet_wait( i2c_master_instance, &packet ) ) != STATUS_OK )
  {
    if ( timeout++ >= MAX_RETRIES )
      break;
  }

  return status;
}

/**
 * \brief Write a single byte to the specified I2C address.
 *
 * Internally uses the ASF function i2c_master_write_packet_wait(..), through
 * SMBus.writeBlock(..)
 *
 * \param [in] i2c_master_instance pointer to i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] data byte to send to the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions; returns
 *                     exactly what i2c_master_write_packet_wait(..) returns
 */

enum status_code smbus_writeByte( struct i2c_master_module *const i2c_master_instance,
                                  uint8_t device_address, uint8_t data )
{
  return smbus_writeBlock( i2c_master_instance, device_address, &data, 1 );
}

/**
 * \brief Read a single byte from the specified I2C address.
 *
 * Internally uses the ASF function i2c_master_read_packet_wait(..), through
 * SMBus.readBlock(..)
 *
 * \param [in] i2c_master_instance pointer to i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [out] data byte received from the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions; returns
 *                     exactly what i2c_master_read_packet_wait(..) returns
 */

enum status_code smbus_readByte( struct i2c_master_module *const i2c_master_instance,
                                 uint8_t device_address, uint8_t* data )
{
  return smbus_readBlock( i2c_master_instance, device_address, data, 1 );
}

/**
 * \brief Write a word to the specified I2C address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] data byte to send to the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_writeWord( struct i2c_master_module *const i2c_master_instance,
                                  uint8_t device_address, uint16_t data )
{
  uint8_t arr[2] = { (uint8_t) (data & 255), (uint8_t) (data >> 8) };

  return smbus_writeBlock( i2c_master_instance, device_address, (uint8_t*) &arr, 2 );
}

/**
 * \brief Read a word from the specified I2C address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [out] data byte received from the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_readWord( struct i2c_master_module *const i2c_master_instance,
                                 uint8_t device_address, uint16_t* data )
{
  enum status_code status = STATUS_OK;

  uint8_t arr[2] = { 255, 255 };

  status = smbus_readBlock( i2c_master_instance, device_address, (uint8_t*) arr, 2 );
  *data = (uint16_t) ( (arr[1] << 8) | arr[0] );

  return status;
}

/**
 * \brief Write a single byte of data (to a specific register) to the specified I2C address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] cmd register address/command to send to the slave
 * \param [in] data byte to send to the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_writeByteData( struct i2c_master_module *const i2c_master_instance,
                                      uint8_t device_address, uint8_t cmd, uint8_t data )
{
  uint8_t arr[2] = { cmd, data };

  return smbus_writeBlock( i2c_master_instance, device_address, (uint8_t*) &arr, 2 );
}

/**
 * \brief Read a single byte of data (from a specific register) from the specified I2C address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] cmd register address/command to send to the slave
 * \param [out] data byte received from the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_readByteData( struct i2c_master_module *const i2c_master_instance,
                                     uint8_t device_address, uint8_t cmd, uint8_t* data )
{
  enum status_code status = STATUS_OK;

  status |= smbus_writeByte( i2c_master_instance, device_address, cmd );
  status |= smbus_readByte( i2c_master_instance, device_address, data );

  return status;
}

/**
 * \brief Write a word of data (to a specific register) to the specified I2C address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] cmd register address/command to send to the slave
 * \param [in] data byte to send to the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_writeWordData( struct i2c_master_module *const i2c_master_instance,
                                      uint8_t device_address, uint8_t cmd, uint16_t data )
{
  uint8_t arr[3] = { cmd, (uint8_t) (data & 255), (uint8_t) (data >> 8) };

  return smbus_writeBlock( i2c_master_instance, device_address, (uint8_t*) &arr, 3 );
}

/**
 * \brief Read a word data (from a specific register) from the specified I2C address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] cmd register address/command to send to the slave
 * \param [out] data byte received from the slave
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_readWordData( struct i2c_master_module *const i2c_master_instance,
                                     uint8_t device_address, uint8_t cmd, uint16_t* data )
{
  enum status_code status = STATUS_OK;

  status |= smbus_writeByte( i2c_master_instance, device_address, cmd );
  status |= smbus_readWord( i2c_master_instance, device_address, data );

  return status;
}

/**
 * \brief Write a block of data (to a specific starting register) to the specified I2C
 *        address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] cmd register address/command to send to the slave
 * \param [in] data block to send to the slave
 * \param [in] count number of bytes in the block of data
 *
 * \note Not really part of the SMBus standard.
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_writeBlockData( struct i2c_master_module *const i2c_master_instance,
                                       uint8_t device_address, uint8_t cmd, uint8_t* data,
                                       uint32_t count )
{
  enum status_code status = STATUS_OK;

  uint8_t* write_data = (uint8_t*) malloc( (count+1)*sizeof(uint8_t) );
  write_data[0] = cmd;
  for ( unsigned int i = 1; i < count+1; ++i )
    write_data[i] = data[i-1];

  status = smbus_writeBlock( i2c_master_instance, device_address, write_data, count+1 );

  free( write_data );

  return status;
}

/**
 * \brief Read a block of data (from a specific starting register) from the specified I2C
 *        address.
 *
 * \param [in] i2c_master_instance pointer to the i2c_master_module instance in your main
 *                                 application
 * \param [in] device_address 7-bit I2C address of the slave
 * \param [in] cmd register address/command to send to the slave
 * \param [out] data byte received from the slave
 * \param [in] count number of bytes to read from the slave
 *
 * \note Not really a part of the SMBus standard.
 *
 * \return status_code enumeration defined in ASF status code abstractions
 */

enum status_code smbus_readBlockData( struct i2c_master_module *const i2c_master_instance,
                                      uint8_t device_address, uint8_t cmd, uint8_t* data,
                                      uint32_t count )
{
  enum status_code status = STATUS_OK;

  status |= smbus_writeByte( i2c_master_instance, device_address, cmd );
  status |= smbus_readBlock( i2c_master_instance, device_address, data, count );

  return status;
}

/**
 * \} end of atmel_samd20_smbus_master_blocking group
 */
