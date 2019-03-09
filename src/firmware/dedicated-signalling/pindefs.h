/**
 * Copyright (C) 2018 Shreyas Vinod
 *
 * This file is a part of the AHTI hardware abstraction layer (ahti-hal).
 *
 * \file pindefs.h
 * \author Shreyas Vinod <shreyas@shreyasvinod.xyz>
 * \date 09 Mar 2019
 *
 * \brief A set of pin abstraction definitions for the AHTI motherboard.
 *        This file corresponds to the Dedicated Signalling Controller unit.
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
