/** \file Config.h
*
* Maps hardware to software
*
* $Name:  $
*/
/*******************************************************************************
* Copyright (c) 2006 Dynastream Innovations Inc.
* THE FOLLOWING EXAMPLE CODE IS INTENDED FOR LIMITED CIRCULATION ONLY.
* 
* Please forward all questions regarding this code to ANT Technical Support.
* 
* Dynastream Innovations Inc.
* 228 River Avenue
* Cochrane, Alberta, Canada
* T4C 2C1
* 
* (P) (403) 932-9292
* (F) (403) 932-6521
* (TF) 1-866-932-9292
* (E) support@thisisant.com
* 
* www.thisisant.com
*
*******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

// Compiler Environment
//#include <msp430x20x3.h>                                    // Target MCU

// Bit Sync Serial Port Definitions
// SEN
#define SYNC_SEN_DIR                         P1DIR
#define SYNC_SEN_IN                          P1IN
#define SYNC_SEN_BIT                         BIT6           // P1.6 
#define SYNC_SEN_IFG                         P1IFG          // Interrupt Flag
#define SYNC_SEN_IES                         P1IES          // Interrupt Edge Select
#define SYNC_SEN_IE                          P1IE           // Interrupt Enable
//------------------------------------------------------------------------------
// SMSGRDY
#define SYNC_SMSGRDY_DIR                     P1DIR
#define SYNC_SMSGRDY_OUT                     P1OUT
#define SYNC_SMSGRDY_BIT                     BIT1           // P1.1
//------------------------------------------------------------------------------
// SIN
#define SYNC_SIN_DIR                         P1DIR
#define SYNC_SIN_BIT                         BIT2           // P1.2 (SOUT on ANT)
#define SYNC_SIN_IN                          P1IN
//------------------------------------------------------------------------------
// SOUT
#define SYNC_SOUT_SEL                        P1SEL
#define SYNC_SOUT_DIR                        P1DIR
#define SYNC_SOUT_BIT                        BIT3           // P1.3 (SIN on ANT)
#define SYNC_SOUT_OUT                        P1OUT
//------------------------------------------------------------------------------
// SCLK
#define SYNC_SCLK_SEL                        P1SEL
#define SYNC_SCLK_DIR                        P1DIR
#define SYNC_SCLK_BIT                        BIT4           // P1.4
#define SYNC_SCLK_IN                         P1IN
#define SYNC_SCLK_IES                        P1IES
#define SYNC_SCLK_IE                         P1IE
#define SYNC_SCLK_IFG                        P1IFG
//------------------------------------------------------------------------------
// SRDY
#define SYNC_SRDY_SEL                        P1SEL
#define SYNC_SRDY_DIR                        P1DIR
#define SYNC_SRDY_OUT                        P1OUT
#define SYNC_SRDY_BIT                        BIT5           // P1.5
//------------------------------------------------------------------------------
#define LED_SEL                              P1SEL
#define LED_DIR                              P1DIR
#define LED_OUT                              P1OUT
#define LED_BIT                              BIT0           // P1.0
//------------------------------------------------------------------------------
// Interrupt Servicing
#define SYNC_PORT1_ISR(func)                 PORT1_ISR(func)
#define SYNC_PORT2_ISR(func)                 PORT2_ISR(func)
//------------------------------------------------------------------------------
// MCU System definitions
// Watchdog
#define SYSTEM_WATCHDOG_TCTL                 WDTCTL         // Watchdog Timer Control
#define SYSTEM_WATCHDOG_PW                   WDTPW          // Watchdog Password
#define SYSTEM_WATCHDOG_HOLD                 WDTHOLD        // Watchdog Timer Hold (Disables Watchdog)
#define SYSTEM_WATCHDOG_WDTIE                WDTIE          // Watchdog Interrupt Enable
#define SYSTEM_WATCHDOG_WDTIFG               WDTIFG         // Watchdog Interrupt Flag
//------------------------------------------------------------------------------
// Clocks
#define SYSTEM_DCO_0                         DCO0           // DCO Select Bit 0
#define SYSTEM_DCO_1                         DCO1           // DCO Select Bit 1
#define SYSTEM_DCO_2                         DCO2           // DCO Select Bit 2 
#define SYSTEM_DCOCTL                        DCOCTL         // DCO Control
#define SYSTEM_BCSCTL1                       BCSCTL1        // Basic Clock System Control 1
#define SYSTEM_BCSCTL2                       BCSCTL2        // Basic Clock System Control 2
#define SYSTEM_BCSCTL3                       BCSCTL3        // Basic Clock System Control 3
#define SYSTEM_RSEL0                         RSEL0          // DCO Resistor Select 0
#define SYSTEM_RSEL1                         RSEL1          // DCO Resistor Select 1
#define SYSTEM_RSEL2                         RSEL2          // DCO Resistor Select 2
#define SYSTEM_RSEL3                         RSEL3          // DCO Resistor Select 3
#define SYSTEM_DCOR                          DCOR           // ROSC control
#define SYSTEM_XTS                           XTS            // LFXT1 control
#define SYSTEM_DIVA0                         DIVA0          // ACLK divide setting low bit
#define SYSTEM_DIVA1                         DIVA1          // ACLK divide setting high bit
#define SYSTEM_XT2OFF                        XT2OFF         // External clock 2 OFF/ON
#define SYSTEM_IE1                           IE1            // Interrupt Enable Register 1
#define SYSTEM_OFIE                          OFIE           // Oscillator Fault Interrupt Enable
#define SYSTEM_IFG1                          IFG1           // Interrupt Flag Register 1
#define SYSTEM_OFIFG                         OFIFG          // Oscillator Fault Interrup Flag
#define MAX_MCLK_FREQ                        (SYSTEM_DCO_0 | SYSTEM_DCO_1 | SYSTEM_DCO_2) // maximum DCO setting
//------------------------------------------------------------------------------
// System Ops
#define SYSTEM_LPM0                          LPM0           // LPM0 enable
#define SYSTEM_LPM0_EXIT                     LPM0_EXIT      // LPM0 disable
//------------------------------------------------------------------------------
// Timer A
#define TIMER_A_TACTL                        TACTL          // Timer A control register
#define TIMER_A_TAR                          TAR            // Timer A counter register
#define TIMER_A_MC                           MC_2           // Timer A mode control (continuous mode)
#define TIMER_A_TASSEL                       TASSEL_2       // Timer A clock select (SMCLK)
#define TIMER_A_ID                           ID_3           // Timer A clock divide select (8)
#define TIMER_A_TAIE                         TAIE           // Timer A Interrupt Enable

#endif 
