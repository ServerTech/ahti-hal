/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * This file is a part of the AHTI hardware abstraction layer (ahti-hal).
 *
 * \file main.c
 * \author Shreyas Vinod <shreyas@shreyasvinod.xyz>
 * \date 28 Apr 2018
 *
 * \brief Dedicated signalling controller (Atmel ATSAMD20E based) for high
 *        performance PWM signals.
 */

#include <asf.h>

#include "pindefs.h"

#define BUFFER_LENGTH 48

/* i2c commands */
#define REG_GET_CHANNEL 0x11
#define REG_SET_CHANNEL 0x12

/* i2c */
static struct i2c_slave_packet packet;
static struct i2c_slave_module pi_bus;
static uint8_t read_buffer[BUFFER_LENGTH];
static uint8_t write_buffer[BUFFER_LENGTH];

/* tc */
static struct tc_module tc1_instance;

static uint16_t pwm_duty_setpoint[PWM_CHANNEL_COUNT];

void init_pibus( void );
void init_tc( void );

/* i2c callbacks */
void pi_bus_read_callback( struct i2c_slave_module *const module );
void pi_bus_write_callback( struct i2c_slave_module *const module );

/* tc callbacks */
void pwm_channel1( struct tc_module *const module_inst );

void init_pibus( void )
{
  struct i2c_slave_config config_i2c_slave;
  i2c_slave_get_config_defaults( &config_i2c_slave );

  config_i2c_slave.address      = PWM_PIBUS_ADDR;
  config_i2c_slave.address_mode = I2C_SLAVE_ADDRESS_MODE_MASK;
  config_i2c_slave.pinmux_pad0  = PWM_PI_PAD0;
  config_i2c_slave.pinmux_pad1  = PWM_PI_PAD1;

  while ( i2c_slave_init( &pi_bus, PWM_PI_MOD, &config_i2c_slave ) != STATUS_OK )
    continue;

  i2c_slave_enable( &pi_bus );

  i2c_slave_register_callback( &pi_bus, pi_bus_read_callback,
                               I2C_SLAVE_CALLBACK_READ_REQUEST );
  i2c_slave_enable_callback( &pi_bus, I2C_SLAVE_CALLBACK_READ_REQUEST );
  i2c_slave_register_callback( &pi_bus, pi_bus_write_callback,
                              I2C_SLAVE_CALLBACK_WRITE_REQUEST );
  i2c_slave_enable_callback( &pi_bus, I2C_SLAVE_CALLBACK_WRITE_REQUEST );
}

void init_tc( void )
{
  struct tc_config config_tc;
  tc_get_config_defaults( &config_tc );

  config_tc.counter_size    = TC_COUNTER_SIZE_16BIT;
  config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
  config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV2;
  config_tc.counter_16_bit.compare_capture_channel[0] = 0xffff;
  config_tc.counter_16_bit.compare_capture_channel[1] = 0xffff;

  config_tc.pwm_channel[0].enabled = true;
  config_tc.pwm_channel[0].pin_out = PWM1;
  config_tc.pwm_channel[0].pin_mux = PWM1_MUX;

  config_tc.pwm_channel[1].enabled = true;
  config_tc.pwm_channel[1].pin_out = PWM2;
  config_tc.pwm_channel[1].pin_mux = PWM2_MUX;

  tc_init( &tc1_instance, PWM1_MOD, &config_tc );
  tc_enable( &tc1_instance );

  // tc_register_callback( &tc1_instance, pwm_channel1, TC_CALLBACK_CC_CHANNEL0 );
  // tc_enable_callback( &tc1_instance, TC_CALLBACK_CC_CHANNEL0 );
}

/* i2c callbacks */

/* master wants to receive data */
void pi_bus_read_callback( struct i2c_slave_module *const module )
{
  /* parse previously received packet */
  uint8_t cmd = read_buffer[0]; /* readability */

  if ( cmd == REG_GET_CHANNEL )
  {
    uint8_t get_channel = read_buffer[1] - 1;
    if ( get_channel < PWM_CHANNEL_COUNT )
    /* get_channel >= 0 implicit due to unsigned */
    {
      write_buffer[0] = pwm_duty_setpoint[get_channel];
    }
    else
    {
      write_buffer[0] = 200;
    }
    packet.data_length = 1;
  }
  else if ( cmd == REG_SET_CHANNEL )
  {
    uint8_t  set_channel = read_buffer[1] - 1;
    uint16_t new_duty    = ( read_buffer[3] << 8 ) | read_buffer[2];
    if ( set_channel < PWM_CHANNEL_COUNT && new_duty <= 65536 )
    /* set_channel >= 0 && new_duty >= 0 implicit due to unsigned */
    {
      pwm_duty_setpoint[set_channel] = new_duty;
      write_buffer[0] = 42;
    }
    else
    {
      write_buffer[0] = 200;
    }
    packet.data_length = 200;
  }

  /* finally, write it to the bus! */
  packet.data = write_buffer;
  if ( i2c_slave_write_packet_job(module, &packet) != STATUS_OK )
  {
    // TODO
  }
}

/* master wants to send data */
void pi_bus_write_callback( struct i2c_slave_module *const module )
{
  packet.data_length = BUFFER_LENGTH;
  packet.data        = read_buffer;

  /* read the packet */
  if ( i2c_slave_read_packet_job(module, &packet) != STATUS_OK )
  {
    // TODO
  }
  
  uint8_t cmd = read_buffer[0]; /* readability */
  

}

/* tc callbacks */

void pwm_channel1( struct tc_module *const module_inst )
{
  static uint16_t i = 32768;

  tc_set_compare_value( module_inst, TC_COMPARE_CAPTURE_CHANNEL_0, i );
  tc_set_compare_value( module_inst, TC_COMPARE_CAPTURE_CHANNEL_1, i );
}

int main( void )
{
  system_init();

  for ( int i = 0; i < PWM_CHANNEL_COUNT; ++i )
  {
    pwm_duty_setpoint[i] = 50;
    pwm_duty_counter[i]  = 32768;
  }

  init_tc();
  system_interrupt_enable_global();
  init_pibus();

  while ( true )
  {
    tc_set_compare_value( &tc1_instance, PWM1_CHANNEL, pwm_duty_setpoint[0] );
    /* THIS IS TEMPORARY! */
    tc_set_compare_value( &tc1_instance, PWM2_CHANNEL, pwm_duty_setpoint[0] );
  }
}
