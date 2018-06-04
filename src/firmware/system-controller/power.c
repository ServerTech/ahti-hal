/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * This file is a part of the AHTI hardware abstraction layer (ahti-hal).
 *
 * \file power.c
 * \author Shreyas Vinod <shreyas@shreyasvinod.xyz>
 * \date 14 Apr 2018
 *
 * \brief Power controller initialisation and management wrappers.
 */

#include <asf.h>
#include <math.h>

#include "defs.h"
#include "notifier.h"
#include "smbus.h"
#include "power.h"

/* i2c commands */
#define REG_OPERATION                0x01
#define REG_ON_OFF_CONFIG            0x02
#define REG_CLEAR_FAULTS             0x03
#define REG_WRITE_PROTECT            0x10

#define REG_VOUT_MODE                0x20
#define REG_VOUT_COMMAND             0x21
#define REG_VOUT_TRIM                0x22
#define REG_VOUT_MARGIN_HIGH         0x25
#define REG_VOUT_MARGIN_LOW          0x26
#define REG_VOUT_DROOP               0x28

#define REG_VOUT_OV_FAULT_LIMIT      0x40
#define REG_VOUT_OV_FAULT_RESPONSE   0x41
#define REG_VOUT_OV_WARN_LIMIT       0x42

#define REG_IOUT_OC_FAULT_LIMIT      0x46
#define REG_IOUT_OC_FAULT_RESPONSE   0x47
#define REG_IOUT_OC_WARN_LIMIT       0x4a

#define REG_OT_FAULT_LIMIT           0x4f
#define REG_OT_FAULT_RESPONSE        0x50
#define REG_OT_WARN_LIMIT            0x51

#define REG_VIN_OV_FAULT_LIMIT       0x55
#define REG_VIN_OV_FAULT_RESPONSE    0x56
#define REG_VIN_OV_WARN_LIMIT        0x57
#define REG_VIN_UV_FAULT_LIMIT       0x59
#define REG_VIN_UV_FAULT_RESPONSE    0x5a
#define REG_VIN_UV_WARN_LIMIT        0x58

#define REG_STATUS_BYTE              0x78
#define REG_STATUS_WORD              0x79
#define REG_STATUS_VOUT              0x7a
#define REG_STATUS_IOUT              0x7b
#define REG_STATUS_INPUT             0x7c
#define REG_STATUS_TEMPERATURE       0x7d
#define REG_STATUS_CML               0x7e

#define REG_READ_VIN                 0x88
#define REG_READ_VOUT                0x8b
#define REG_READ_IOUT                0x8c
#define REG_READ_TEMPERATURE_1       0x8d
#define REG_READ_TEMPERATURE_2       0x8e
#define REG_READ_POUT                0x96

double power_parseLinearFormatBytes( uint8_t lsb, int8_t msb );
double power_parseLinearFormat( uint16_t word );
double power_getCumulativeMeasurement( Power_t* pc, uint8_t cmd );
double power_getMeanMeasurement( Power_t* pc, uint8_t cmd );
double power_getVin( Power_t* pc );
double power_getIout( Power_t* pc );
double power_getPout( Power_t* pc );
double power_getVout( Power_t* pc );
Power_status power_switchOn( Power_t* pc );
Power_status power_switchOff( Power_t* pc );

double power_parseLinearFormatBytes( uint8_t lsb, int8_t msb )
{
  int8_t n = msb >> 3;
  int16_t y = ( ( msb & 7 ) << 8 ) | lsb;
  return (double) ( y * powl(2, n) );
}

double power_parseLinearFormat( uint16_t word )
{
  return power_parseLinearFormatBytes( (uint8_t) (word & 255), (uint8_t) (word >> 8) );
}

double power_getCumulativeMeasurement( Power_t* pc, uint8_t cmd )
{
  double   meas = 0;
  uint16_t tmp16;

  for ( int i = 0; i < pc->module_count; ++i )
  {
    if ( SMBus.readWordData( pc->pmbus, pc->module_addr[i], cmd, &tmp16 ) != STATUS_OK )
    {
      pc->status = POWER_PMBUS;
      #ifdef DEBUG_MODE
        Notifier.error( SYSTEM_POWER, pc->status );
      #endif /* DEBUG_MODE */
      return -1;
    }

    meas += power_parseLinearFormat( tmp16 );
  }

  return meas;
}

double power_getMeanMeasurement( Power_t* pc, uint8_t cmd )
{
  return power_getCumulativeMeasurement( pc, cmd ) / pc->module_count;
}

double power_getVin( Power_t* pc )
{
  double vin;

  vin = power_getMeanMeasurement( pc, REG_READ_VIN );
  if ( vin < 0 ) return -1;

  if ( vin < POWER_VIN_RANGE_MIN || vin > POWER_VIN_RANGE_MAX )
  {
    pc->status = POWER_VIN_OUT_OF_RANGE;
    #ifdef DEBUG_MODE
      Notifier.error( SYSTEM_POWER, pc->status );
    #endif /* DEBUG_MODE */
    return -1;
  }

  return vin;
}

double power_getIout( Power_t* pc )
{
  double iout;

  iout = power_getCumulativeMeasurement( pc, REG_READ_IOUT );
  if ( iout < 0 ) return -1;

  if ( iout < POWER_IOUT_RANGE_MIN || iout > POWER_IOUT_RANGE_MAX )
  {
    pc->status = POWER_IOUT_OUT_OF_RANGE;
    #ifdef DEBUG_MODE
      Notifier.error( SYSTEM_POWER, pc->status );
    #endif /* DEBUG_MODE */
    return -1;
  }

  return iout;
}

double power_getPout( Power_t* pc )
{
  double pout;

  pout = power_getCumulativeMeasurement( pc, REG_READ_POUT );
  if ( pout < 0 ) return -1;

  if ( pout < POWER_POUT_RANGE_MIN || pout > POWER_POUT_RANGE_MAX )
  {
    pc->status = POWER_POUT_OUT_OF_RANGE;
    #ifdef DEBUG_MODE
      Notifier.error( SYSTEM_POWER, pc->status );
    #endif /* DEBUG_MODE */
    return -1;
  }

  return pout;
}

double power_getVout( Power_t* pc )
{
  double   vout = 0;
  uint8_t  tmp;
  uint16_t tmp16;
  int8_t   exp;
  double   vout_tmp;

  for ( int i = 0; i < pc->module_count; ++i ) 
  {
    if ( SMBus.readByteData( pc->pmbus, pc->module_addr[i], REG_VOUT_MODE, &tmp ) != STATUS_OK )
    {
      pc->status = POWER_PMBUS;
      #ifdef DEBUG_MODE
        Notifier.error( SYSTEM_POWER, pc->status );
      #endif /* DEBUG_MODE */
      return -1;
    }

    if ( tmp & 16 ) exp = (int8_t) (224 | (tmp & 31));
    else exp = (int8_t) (tmp & 15);

    if ( exp < -16 || exp > 15 )
    {
      pc->status = POWER_EXP_OUT_OF_RANGE;
      #ifdef DEBUG_MODE
        Notifier.error( SYSTEM_POWER, pc->status );
      #endif /* DEBUG_MODE */
      return -1;
    }

    if ( SMBus.readWordData( pc->pmbus, pc->module_addr[i], REG_READ_VOUT, &tmp16 ) != STATUS_OK )
    {
      pc->status = POWER_PMBUS;
      #ifdef DEBUG_MODE
        Notifier.error( SYSTEM_POWER, pc->status );
      #endif /* DEBUG_MODE */
      return -1;
    }

    vout_tmp = tmp16 * pow(2,exp);

    if ( vout_tmp < POWER_VOUT_RANGE_MIN || vout_tmp > POWER_VOUT_RANGE_MAX )
    {
      pc->status = POWER_VOUT_OUT_OF_RANGE;
      #ifdef DEBUG_MODE
        Notifier.error( SYSTEM_POWER, pc->status );
      #endif /* DEBUG_MODE */
      return -1;
    }

    vout += vout_tmp;
  }

  vout /= pc->module_count;
  return vout;
}

Power_status power_switchOn( Power_t* pc )
{
  Power_status status = POWER_INIT;

  double vin, vout;

  if ( (vin = power_getVin( pc )) < 0 ) return pc->status;
  if ( abs( (double) (POWER_VIN_NOMINAL - vin) ) > (POWER_VIN_TOLERANCE/100)*POWER_VIN_NOMINAL )
  {
    pc->status = POWER_VIN_FAULT;
    #ifdef DEBUG_MODE
      Notifier.error( SYSTEM_POWER, pc->status );
    #endif /* DEBUG_MODE */
    return pc->status;
  }

  

}
