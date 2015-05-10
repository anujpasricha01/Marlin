/***********************************************************************************
* For use with the Texas Instruments ADS1115 16-bit ADC
*
* @author Ricky Rininger 02/03/2015
*
***********************************************************************************/

#ifndef ADS1115_H
#define ADS1115_H

#include "Arduino.h"
#include <Wire.h>
#include "Configuration.h"

/* I2C ADDRESS */
/*-------------*/
#define ADS1115_I2C_ADDRESS 0x48 // CHANGE THIS IF YOUR ADDRESS IS NOT 0x48 !!!!!

/* CONVERSION DELAY (ms) */
/*-----------------------*/
#define ADS1115_CONVERSION_DELAY 8     // Delay to allow conversion to finish

/* POINTER REGISTER DEFINITIONS */
/*------------------------------*/
#define ADS1115_CONVERSION_REG  0X00 // Read Only
#define ADS1115_CONFIG_REG      0x01
#define ADS1115_LO_THRESH_REG   0x02
#define ADS1115_HI_THRESH_REG   0x03

/* CONFIG REGISTER DEFINITIONS */
/*-----------------------------*/
#define ADS1115_OS_SINGLE       0x8000 // Start conversion (WRITE)
#define ADS1115_OS_BUSY         0x0000 // Bit = 0 if conversion in progress (READ)
#define ADS1115_OS_NOTBUSY      0x8000 // Bit = 1 if no conversion in progress (READ)

#define ADS1115_MUX_DIFF_0_1    0x0000 // Differential P = 0, N = 1  
#define ADS1115_MUX_DIFF_0_3    0x1000 // Differential P = 0, N = 3
#define ADS1115_MUX_DIFF_1_3    0x2000 // Differential P = 1, N = 3
#define ADS1115_MUX_DIFF_2_3    0x3000 // Differential P = 2, N = 3
#define ADS1115_MUX_SINGLE_00   0x4000 // Channel 0 single-ended operation
#define ADS1115_MUX_SINGLE_01   0x5000 // Channel 1 single-ended operation
#define ADS1115_MUX_SINGLE_02   0x6000 // Channel 2 single-ended operation
#define ADS1115_MUX_SINGLE_03   0x7000 // Channel 3 single-ended operation

#define ADS1115_PGA_6144        0x0000 // PGA Gain = 2/3
#define ADS1115_PGA_4096        0x0200 // PGA Gain = 1
#define ADS1115_PGA_2048        0x0400 // PGA Gain = 2 (DEFAULT)
#define ADS1115_PGA_1024        0x0600 // PGA Gain = 4
#define ADS1115_PGA_0512        0x0800 // PGA Gain = 8
#define ADS1115_PGA_0256        0x0A00 // PGA Gain = 16

#define ADS1115_MODE_CONT       0x0000 // Continuous conversion mode
#define ADS1115_MODE_SINGLE     0x0100 // Single-shot mode (DEFAULT)

#define ADS1115_DR_008          0x0000 // Data Rate = 8SPS
#define ADS1115_DR_016          0x0020 // Data Rate = 16SPS
#define ADS1115_DR_032          0x0040 // Data Rate = 32SPS
#define ADS1115_DR_064          0x0060 // Data Rate = 64SPS
#define ADS1115_DR_128          0x0080 // Data Rate = 128SPS (DEFAULT)
#define ADS1115_DR_250          0x00A0 // Data Rate = 250SPS
#define ADS1115_DR_475          0x00C0 // Data Rate = 475SPS
#define ADS1115_DR_860          0x00E0 // Date Rate = 860SPS

#define ADS1115_COMP_MODE_TRAD  0x0000 // Traditional Comparator w/ hysteresis (DEFAULT)
#define ADS1115_COMP_MODE_WIND  0x0010 // Window Comparator

#define ADS1115_COMP_POL_ACTLO  0x0000 // Comparator output active low (DEFAULT)
#define ADS1115_COMP_POL_ACTHI  0x0008 // Comparator output active high

#define ADS1115_COMP_NONLATCH   0x0000 // Non-latching comparator (DEFAULT)
#define ADS1115_COMP_LATCHING   0x0004 // Latching comparator

#define ADS1115_COMP_QUE_1CONV  0x0000 // Assert after one conversion
#define ADS1115_COMP_QUE_2CONV  0x0001 // Assert after two conversions
#define ADS1115_COMP_QUE_4CONV  0x0002 // Assert after four conversions
#define ADS1115_COMP_DISABLE    0x0003 // Disable comparator (DEFAULT)
/*--------------------------------------------------------------------------------*/
    
/* M-Code Definitions  */
/*---------------------*/
// M234 - Get raw data from ADC

#if EXT_ADC_MODE == 1 // Single-Ended Mode

#define EXT_ADC_RAW_0	readADC_SingleEnded(0) // read from channel 0
#define EXT_ADC_RAW_1	readADC_SingleEnded(1) // read from channel 1
#define EXT_ADC_RAW_2	readADC_SingleEnded(2) // read from channel 2
#define EXT_ADC_RAW_3	readADC_SingleEnded(3) // read from channel 3

#elif EXT_ADC_MODE == 2 // Differential Mode

#define EXT_ADC_RAW_0	readADC_Differential(0, 1) // read from ch0 and ch1
#define EXT_ADC_RAW_1	readADC_Differential(1, 3) // read from ch1 and ch3
#define EXT_ADC_RAW_2	readADC_Differential(2, 3) // read from ch2 and ch3
#define EXT_ADC_RAW_3	readADC_Differential(0, 3) // read from ch0 and ch3

#endif

//M235 All Single Ended
#define EXT_ADC_READ_0	get_dist(0)	// M234 - Processes raw ADC value and returns distance
#define EXT_ADC_READ_1	get_dist(1)	// M234 - Processes raw ADC value and returns distance
#define EXT_ADC_READ_2	get_dist(2)	// M234 - Processes raw ADC value and returns distance
#define EXT_ADC_READ_3	get_dist(3)	// M234 - Processes raw ADC value and returns distance

/* Definitions for calculations */
/*------------------------------*/

// TODO: set this to auto-configure conversion factor based on a value set in the config.h file
#define CONV_FACTOR     0.000188    // (6.144V / 32767 bits) 
#define VOLT_TO_DIST    2000        // (10000 um / 5V)



/* Function Prototypes */
/*=====================*/

uint16_t readADC_SingleEnded(uint8_t channel);

uint16_t readADC_Differential(uint8_t first_channel, uint8_t second_channel);

void regWrite(uint8_t address, uint8_t reg, uint16_t value);

uint16_t regRead(uint8_t address, uint8_t reg);

uint16_t get_dist(uint8_t channel);

#endif
