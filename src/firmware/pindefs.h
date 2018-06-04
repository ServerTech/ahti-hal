/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * This file is a part of the AHTI hardware abstraction layer (ahti-hal).
 *
 * \file pindefs.h
 * \author Shreyas Vinod <shreyas@shreyasvinod.xyz>
 * \date 31 Mar 2018
 *
 * \brief A set of pin abstraction definitions for the AHTI motherboard.
 */

#ifndef PINDEFS_H_
#define PINDEFS_H_

/**
 * \defgroup pindefs Pin Definitions
 * \brief Pin definition abstractions.
 * \{
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup pindefs_system_controller System Controller Pin Definitions
 * \brief Pin definition abstractions for the system controller unit.
 * \{
 */

/* ################################################## */
/*                    SIGNALLING                      */
/* ################################################## */

/* 3V3 signalling */
#define SIG1 PIN_PA01          /* pin 2  */
#define SIG2 PIN_PA03          /* pin 4  */
#define SIG3 PIN_PB04          /* pin 5  */
#define SIG4 PIN_PB06          /* pin 9  */
#define SIG5 PIN_PB07          /* pin 10 */
#define SIG6 PIN_PB09          /* pin 12 */
#define SIG7 PIN_PA04          /* pin 13 */
#define SIG8 PIN_PA06          /* pin 15 */

/* 5V signalling */
#define FSIG1 PIN_PA00         /* pin 1  */
#define FSIG2 PIN_PA02         /* pin 3  */
#define FSIG3 PIN_PB05         /* pin 6  */
#define FSIG4 PIN_PB08         /* pin 11 */
#define FSIG5 PIN_PA05         /* pin 14 */
#define FSIG6 PIN_PA07         /* pin 16 */

/* ################################################## */
/*                   POWER CONTROL                    */
/* ################################################## */

/* 5V power primary control */
#define PFI PIN_PA25           /* pin 46 */

/* 12V power primary control */
#define PTW PIN_PA12           /* pin 29 */

/* 12V power secondary control */
#define STW1 PIN_PB15          /* pin 28 */
#define STW2 PIN_PB14          /* pin 27 */
#define STW3 PIN_PB11          /* pin 24 */
#define STW4 PIN_PB10          /* pin 23 */
#define STW5 PIN_PA11          /* pin 20 */
#define STW6 PIN_PA10          /* pin 19 */

/* 12V power I2C addresses */
// #define STW1_ADDR 0x2a         /*  68k  33k */
// #define STW2_ADDR 0x2c         /*  68k  68k */
#define STW3_ADDR 0x2e         /*  68k 150k */
#define STW4_ADDR 0x3d         /* 150k  22k */
#define STW5_ADDR 0x3e         /* 150k  33k */
#define STW6_ADDR 0x40         /* 150k  68k */
/* true addresses of modules with known addressing failure */
#define STW1_ADDR 0x22
#define STW2_ADDR 0x24

/* ################################################## */
/*                      I2C BUSES                     */
/* ################################################## */

/* system bus */
#define SYS_MOD SERCOM0
#define SYS_SDA PIN_PA08       /* pin 17 */
#define SYS_SCL PIN_PA09       /* pin 18 */
#define SYS_PAD0 PINMUX_PA08C_SERCOM0_PAD0
#define SYS_PAD1 PINMUX_PA09C_SERCOM0_PAD1

/* pi bus  */
#define PI_MOD  SERCOM4
#define PI_SDA  PIN_PB12       /* pin 25 */
#define PI_SCL  PIN_PB13       /* pin 26 */
#define PI_PAD0 PINMUX_PB12C_SERCOM4_PAD0
#define PI_PAD1 PINMUX_PB13C_SERCOM4_PAD1

#define SYS_PIBUS_ADDR 0x17

/* expansion bus */
#define EXP_MOD SERCOM5
#define EXP_SDA PIN_PB30       /* pin 59 */
#define EXP_SCL PIN_PB31       /* pin 60 */
#define EXP_PAD0 PINMUX_PB30D_SERCOM5_PAD0
#define EXP_PAD1 PINMUX_PB31D_SERCOM5_PAD1

/* ################################################## */
/*                   STEPPER CONTROL                  */
/* ################################################## */

/* channel 1 */
#define S1_DIR  PIN_PA13       /* pin 30 */
#define S1_STP  PIN_PA14       /* pin 31 */
#define S1_NSLP PIN_PA15       /* pin 32 */
#define S1_NRST PIN_PA16       /* pin 35 */
#define S1_MS1  PIN_PA19       /* pin 38 */
#define S1_MS2  PIN_PA18       /* pin 37 */
#define S1_MS3  PIN_PA17       /* pin 36 */

/* channel 2 */
#define S2_DIR  PIN_PB16       /* pin 39 */
#define S2_STP  PIN_PB17       /* pin 40 */
#define S2_NSLP PIN_PA20       /* pin 41 */
#define S2_NRST PIN_PA21       /* pin 42 */
#define S2_MS1  PIN_PA24       /* pin 45 */
#define S2_MS2  PIN_PA23       /* pin 44 */
#define S2_MS3  PIN_PA22       /* pin 43 */

/* ################################################## */
/*                    MISC. CONTROL                   */
/* ################################################## */

/* fan */
#define FAN PIN_PB00           /* pin 61 */
/* lights */
#define L1  PIN_PB01           /* pin 62 */
#define L2  PIN_PB02           /* pin 63 */
#define L3  PIN_PB03           /* pin 64 */
/* status LEDs */
#define PWR_OK PIN_PA28        /* pin 53 */
#define CON_OK PIN_PA27        /* pin 51 */
#define REM_OK PIN_PB23        /* pin 50 */
#define ARMED  PIN_PB22        /* pin 49 */

/**
 * \} end of pindefs_system_controller
 */

/**
 * \defgroup pindefs_dedicated_signalling Dedicated Signalling Controller Pin Definitions
 * \brief Pin definition abstractions for the dedicated signalling controller unit.
 * \{
 */

/* ################################################## */
/*                      PWM LINES                     */
/* ################################################## */

#define PWM_CHANNEL_COUNT 12

#define PWM1  PIN_PA07         /* pin 8   TC1_1 */
#define PWM2  PIN_PA06         /* pin 7   TC1_0 */
#define PWM3  PIN_PA05         /* pin 6   TC0_1 */
#define PWM4  PIN_PA04         /* pin 5   TC0_0 */
#define PWM5  PIN_PA01         /* pin 2   TC2_1 */
#define PWM6  PIN_PA00         /* pin 1   TC2_0 */
#define PWM7  PIN_PA25         /* pin 24  TC5_1 */
#define PWM8  PIN_PA24         /* pin 23  TC5_0 */
#define PWM9  PIN_PA23         /* pin 22  TC4_1 */
#define PWM10 PIN_PA22         /* pin 21  TC4_0 */
#define PWM11 PIN_PA19         /* pin 20  TC3_1 */
#define PWM12 PIN_PA18         /* pin 19  TC3_0 */

#define PWM1_MUX  PINMUX_PA07F_TC1_WO1
#define PWM2_MUX  PINMUX_PA06F_TC1_WO0
#define PWM3_MUX  PINMUX_PA05F_TC0_WO1
#define PWM4_MUX  PINMUX_PA04F_TC0_WO0
#define PWM5_MUX  PINMUX_PA01F_TC2_WO1
#define PWM6_MUX  PINMUX_PA00F_TC2_WO0
#define PWM7_MUX  PINMUX_PA25F_TC5_WO1
#define PWM8_MUX  PINMUX_PA24F_TC5_WO0
#define PWM9_MUX  PINMUX_PA23F_TC4_WO1
#define PWM10_MUX PINMUX_PA22F_TC4_WO0
#define PWM11_MUX PINMUX_PA19F_TC3_WO1
#define PWM12_MUX PINMUX_PA18F_TC3_WO0

#define PWM1_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_1
#define PWM2_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_0
#define PWM3_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_1
#define PWM4_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_0
#define PWM5_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_1
#define PWM6_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_0
#define PWM7_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_1
#define PWM8_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_0
#define PWM9_CHANNEL  TC_COMPARE_CAPTURE_CHANNEL_1
#define PWM10_CHANNEL TC_COMPARE_CAPTURE_CHANNEL_0
#define PWM11_CHANNEL TC_COMPARE_CAPTURE_CHANNEL_1
#define PWM12_CHANNEL TC_COMPARE_CAPTURE_CHANNEL_0

#define PWM1_MOD  TC1
#define PWM2_MOD  TC1
#define PWM3_MOD  TC0
#define PWM4_MOD  TC0
#define PWM5_MOD  TC2
#define PWM6_MOD  TC2
#define PWM7_MOD  TC5
#define PWM8_MOD  TC5
#define PWM9_MOD  TC4
#define PWM10_MOD TC4
#define PWM11_MOD TC3
#define PWM12_MOD TC3

/* ################################################## */
/*                      I2C BUSES                     */
/* ################################################## */

/* system bus */
#define PWM_SYS_MOD SERCOM0
#define PWM_SYS_SDA PIN_PA08   /* pin 11 */
#define PWM_SYS_SCL PIN_PA09   /* pin 12 */
#define PWM_SYS_PAD0 PINMUX_PA08C_SERCOM0_PAD0
#define PWM_SYS_PAD1 PINMUX_PA09C_SERCOM0_PAD1

#define PWM_SYSBUS_ADDR 0x17

/* pi bus */
#define PWM_PI_MOD  SERCOM1
#define PWM_PI_SDA  PIN_PA16   /* pin 17 */
#define PWM_PI_SCL  PIN_PA17   /* pin 18 */
#define PWM_PI_PAD0 PINMUX_PA16C_SERCOM1_PAD0
#define PWM_PI_PAD1 PINMUX_PA17C_SERCOM1_PAD1

#define PWM_PIBUS_ADDR 0x18

/* ################################################## */
/*                    MISC. CONTROL                   */
/* ################################################## */

#define LED1 PIN_PA10          /* pin 13 */
#define LED2 PIN_PA11          /* pin 14 */
#define LED3 PIN_PA14          /* pin 15 */
#define LED4 PIN_PA15          /* pin 16 */

/**
 * \} end of pindefs_dedicated_signalling
 */

/**
 * \} end of pindefs
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PINDEFS_H_ */
