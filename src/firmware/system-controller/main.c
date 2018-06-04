#include <asf.h>
#include <math.h>

#include "pindefs.h"
#include "smbus.h"

#define BUFFER_LENGTH 48 /* in bytes (needs to be greater than ID_LENGTH */
#define NAME_LENGTH   22 /* in bytes */

const char MY_NAME[NAME_LENGTH] = "AHTI System Controller";
const uint8_t MY_ID = 1;

struct i2c_master_module sys_bus;
struct i2c_slave_module  pi_bus;

struct i2c_slave_packet  packet;

uint8_t read_buffer[BUFFER_LENGTH];
uint8_t write_buffer[BUFFER_LENGTH];

/* status variabls */
volatile bool status_fan   = 1;
volatile bool status_power = 1;

/* SMBus commands */
#define REG_YOUR_NAME      0x01 /* read block */
#define REG_ID             0x02 /* read byte  */
#define REG_UPDATE         0x03 /* read block */
#define REG_FAN            0x11 /* write byte */
#define REG_POWER          0x12 /* write byte */

/* proto */
void portConfig( int pin, int direction );
void initSysBys( void );
void initPiBus( void );
void piBusReadCallback( struct i2c_slave_module *const module );
void piBusWriteCallback( struct i2c_slave_module *const module );

void portConfig( int pin, int direction )
{
  struct port_config pin_conf;
  port_get_config_defaults( &pin_conf );
  pin_conf.direction = direction;
  port_pin_set_config( pin, &pin_conf );
}

void initSysBus( void )
{
  struct i2c_master_config config_i2c_master;
  i2c_slave_get_config_defaults( &config_i2c_slave );
  config_i2c_slave.baud_rate    = 400;
  config_i2c_master.pinmux_pad0 = SYS_PAD0;
  config_i2c_master.pinmux_pad1 = SYS_PAD1;
  /* watchdog (if set up) will detect an init failure that lasts for too long */
  while ( i2c_master_init( &sys_bus, SYS_MOD, &config_i2c_master ) != STATUS_OK )
    continue;
  i2c_master_enable( &sys_bus ); 
}

void initPiBus( void )
{
  struct i2c_slave_config config_i2c_slave;
  i2c_slave_get_config_defaults( &config_i2c_slave );
  config_i2c_slave.address      = SYS_PIBUS_ADDR;
  config_i2c_slave.address_mode = I2C_SLAVE_ADDRESS_MODE_MASK;
  config_i2c_slave.pinmux_pad0  = PI_PAD0;
  config_i2c_slave.pinmux_pad1  = PI_PAD1;
  while ( i2c_slave_init( &pi_bus, PI_MOD, &config_i2c_slave ) != STATUS_OK )
    continue;
  i2c_slave_enable( &pi_bus );
  i2c_slave_register_callback( &pi_bus, piBusReadCallback,
                               I2C_SLAVE_CALLBACK_READ_REQUEST );
  i2c_slave_enable_callback( &pi_bus, I2C_SLAVE_CALLBACK_READ_REQUEST );
  i2c_slave_register_callback( &pi_bus, piBusWriteCallback,
                               I2C_SLAVE_CALLBACK_WRITE_REQUEST );
  i2c_slave_enable_callback( &pi_bus, I2C_SLAVE_CALLBACK_WRITE_REQUEST );
}

/* master wants to receive data */
void piBusReadCallback( struct i2c_slave_module *const module )
{
  /* parse previously received packet */
  uint8_t cmd = read_buffer[0]; /* readability */

  /* master asks for my name! */
  if ( cmd == REG_YOUR_NAME )
  {
    for ( uint32_t i = 0; i < NAME_LENGTH; ++i )
      write_buffer[i] = MY_NAME[i];
    packet.data_length = NAME_LENGTH;
  }
  /* master asks for my ID! */
  else if ( cmd == REG_ID )
  {
    write_buffer[0] = MY_ID;
    packet.data_length = 1;
  }
  /* master wants to receive updates! */
  else if ( cmd == REG_UPDATE )
  {
    // TODO
  }
  /* master wants to know the fan's status! */
  else if ( cmd == REG_FAN )
  {
    write_buffer[0] = status_fan;
    packet.data_length = 1;
  }
  /* master wants to know the power system's status! */
  else if ( cmd == REG_POWER )
  {
    write_buffer[0] = status_power;
    packet.data_length = 1;
  }
  /* oopsie, master made a mistake! :( */
  else
  {
    write_buffer[0] = 99;
    packet.data_length = 1;
  }

  /* finally, post (write) the love letter to the master! */
  packet.data = write_buffer;
  if ( i2c_slave_write_packet_job( module, &packet ) != STATUS_OK )
  {
    // TODO in the future
  }
}

/* master wants to send data */
void piBusWriteCallback( struct i2c_slave_module *const module )
{
  packet.data_length = BUFFER_LENGTH;
  packet.data        = read_buffer;
  /* read the packet */
  if ( i2c_slave_read_packet_job(module, &packet) != STATUS_OK )
  {
    // TODO in the future
  }
  
  uint8_t cmd = read_buffer[0]; /* readability */

  /* master wants to mess with my fans! */
  if ( cmd == REG_FAN )
  {
    if ( read_buffer[1] )
    {
      // TODO - turn the fan on
    }
    else
    {
      // TODO - turn the fan off
    }
  }
  /* master wants pooooooower! :o */
  else if ( cmd == REG_POWER )
  {
    if ( read_buffer[1] )
    {
      // TODO - turn the power on
    }
    else
    {
      // TODO - turn the power off
    }
  }
}

int main( void )
{
  system_init();

  initSysBus();
  initPiBus();

  portConfig( PTW, PORT_PIN_DIR_OUTPUT );
  portConfig( FAN, PORT_PIN_DIR_OUTPUT );

  while ( true )
  {
    
  }
}
