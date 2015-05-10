/***********************************************************************************
* Contains all function definitions required to operate the ADS1x15 ADC
*
* @author Ricky Rininger 02/11/2015
*   
***********************************************************************************/
#include "Configuration.h"

#ifdef EXT_ADC

#if EXT_ADC == 1
    #include "ADS1115.h"
#elif EXT_ADC ==2
    #include "ADS1015.h"
#endif

uint16_t ADC_val = 0;
float distance = 0;
#if EXT_ADC == 1
    float linear_offset = 5.757;    // linear coefficient of offset that was determined with empirical data
    float const_offset  = 1.57;     // constant term of offset
#elif EXT_ADC == 2
    float linear_offset = 0;
    float const_offset = 0;
#endif

/*================================================================================*/
/*                   GET SINGLE READING FROM ADC (Single-Ended)                   */
/*================================================================================*/

uint16_t readADC_SingleEnded(uint8_t channel) {
    Wire.begin();
    if(channel > 3) {
        return 0;
    }
    // general values for config register
    #if EXT_ADC == 1
        uint16_t config =   ADS1115_MODE_SINGLE | ADS1115_DR_128 |
                            ADS1115_COMP_MODE_TRAD | ADS1115_COMP_POL_ACTLO |
                            ADS1115_COMP_NONLATCH | ADS1115_COMP_DISABLE;
        // set PGA gain
        config |= ADS1115_PGA_6144;

        // channel selection
        switch(channel) {
        case 0:
            config |= ADS1115_MUX_SINGLE_00;
            break;
        case 1:
            config |= ADS1115_MUX_SINGLE_01;
            break;
        case 2:
            config |= ADS1115_MUX_SINGLE_02;
            break;
        case 3:
            config |= ADS1115_MUX_SINGLE_03;
            break;
        default:
            return 0;
        }

        // set conversion bit
        config |= ADS1115_OS_SINGLE;

        // write config settings to config register
        regWrite(ADS1115_I2C_ADDRESS, ADS1115_CONFIG_REG, config);
    
       // delay for 8ms (wait for conversion to finish)
        delay(ADS1115_CONVERSION_DELAY);
    
        ADC_val = regRead(ADS1115_I2C_ADDRESS, ADS1115_CONVERSION_REG);
    
      #elif EXT_ADC == 2
        uint16_t config =   ADS1015_MODE_SINGLE | ADS1015_DR_1600 |
                            ADS1015_COMP_MODE_TRAD | ADS1015_COMP_POL_ACTLO |
                            ADS1015_COMP_NONLATCH | ADS1015_COMP_DISABLE;   
        // set PGA gain
        config |= ADS1015_PGA_6144;  

        // channel selection
        switch(channel) {
        case 0:
            config |= ADS1015_MUX_SINGLE_00;
            break;
        case 1:
            config |= ADS1015_MUX_SINGLE_01;
            break;
        case 2:
            config |= ADS1015_MUX_SINGLE_02;
            break;
        case 3:
            config |= ADS1015_MUX_SINGLE_03;
            break;
        default:
            return 0;
        }
              
        // set conversion bit
        config |= ADS1015_OS_SINGLE;

        // write config settings to config register
        regWrite(ADS1015_I2C_ADDRESS, ADS1015_CONFIG_REG, config);
    
        // delay for 8ms (wait for conversion to finish)
        delay(ADS1015_CONVERSION_DELAY);
    
        ADC_val = regRead(ADS1015_I2C_ADDRESS, ADS1015_CONVERSION_REG);

    #endif

    return ADC_val;
}

uint16_t get_dist(uint8_t channel) {

    uint16_t val_raw = 0;

    val_raw = readADC_SingleEnded(channel);

    // THEORETICALLY, the formula for distance should be distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST)
    // HOWEVER, empirical data has shown that a linear offset exists such that ...
    // distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - offset  , or
    // distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - (mx + b)
    // where m = linear_offset, x = (val_raw * CONV_FACTOR), and b = const_offset

    distance = (val_raw * CONV_FACTOR * VOLT_TO_DIST) - ((linear_offset * val_raw * CONV_FACTOR) - const_offset);
    
    return distance;
}

/*================================================================================*/
/*                   GET SINGLE READING FROM ADC (Differential)                   */
/*================================================================================*/

uint16_t readADC_Differential(uint8_t first_channel, uint8_t second_channel) {
    
     // general values for config register
    #if EXT_ADC == 1
        uint16_t config =   ADS1115_MODE_SINGLE | ADS1115_DR_128 |
                            ADS1115_COMP_MODE_TRAD | ADS1115_COMP_POL_ACTLO |
                            ADS1115_COMP_NONLATCH | ADS1115_COMP_DISABLE;
        // set PGA gain
        config |= ADS1115_PGA_6144;

        // channel selection
        if(first_channel == 0 && second_channel == 1) {
            config |= ADS1115_MUX_DIFF_0_1;
        }
        else if (first_channel == 0 && second_channel == 3) {
            config |= ADS1115_MUX_DIFF_0_3;
        }
        else if (first_channel == 1 && second_channel == 3) {
            config |= ADS1115_MUX_DIFF_1_3;
        }
        else if (first_channel == 2 && second_channel == 3) {
            config |= ADS1115_MUX_DIFF_2_3;
        }
        else {
            return 0;
        }

        // set conversion bit
        config |= ADS1115_OS_SINGLE;

        // write config settings to config register
        regWrite(ADS1115_I2C_ADDRESS, ADS1115_CONFIG_REG, config);
    
        // delay for 8ms (wait for conversion to finish)
        delay(ADS1115_CONVERSION_DELAY);
    
        ADC_val = regRead(ADS1115_I2C_ADDRESS, ADS1115_CONVERSION_REG);

    #elif EXT_ADC == 2
        uint16_t config =   ADS1015_MODE_SINGLE | ADS1015_DR_1600 |
                            ADS1015_COMP_MODE_TRAD | ADS1015_COMP_POL_ACTLO |
                            ADS1015_COMP_NONLATCH | ADS1015_COMP_DISABLE;   
        // set PGA gain
        config |= ADS1015_PGA_6144;       

        // channel selection
        if(first_channel == 0 && second_channel == 1) {
            config |= ADS1015_MUX_DIFF_0_1;
        }
        else if (first_channel == 0 && second_channel == 3) {
            config |= ADS1015_MUX_DIFF_0_3;
        }
        else if (first_channel == 1 && second_channel == 3) {
            config |= ADS1015_MUX_DIFF_1_3;
        }
        else if (first_channel == 2 && second_channel == 3) {
            config |= ADS1015_MUX_DIFF_2_3;
        }
        else {
            return 0;
        }

        // set conversion bit
        config |= ADS1015_OS_SINGLE;

        // write config settings to config register
        regWrite(ADS1015_I2C_ADDRESS, ADS1015_CONFIG_REG, config);
    
        // delay for 8ms (wait for conversion to finish)
        delay(ADS1015_CONVERSION_DELAY);
    
        ADC_val = regRead(ADS1015_I2C_ADDRESS, ADS1015_CONVERSION_REG);
            

    #endif

// THEORETICALLY, the formula for distance should be distance = (ADC_val * CONV_FACTOR * VOLT_TO_DIST)
    // HOWEVER, empirical data has shown that a linear offset exists such that ...
    // distance = (ADC_val * CONV_FACTOR * VOLT_TO_DIST) - offset  , or
    // distance = (ADC_val * CONV_FACTOR * VOLT_TO_DIST) - (mx + b)
    // where m = linear_offset, x = (ADC_val * CONV_FACTOR), and b = const_offset
    distance = (ADC_val * CONV_FACTOR * VOLT_TO_DIST) - ((linear_offset * ADC_val * CONV_FACTOR) - const_offset);
    
    return distance;
}

/*================================================================================*/
/*                               WRITE TO A REGISTER                              */
/*================================================================================*/
void regWrite(uint8_t address, uint8_t reg, uint16_t value) {
    Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);               // select register
    Wire.write((uint8_t)(value >> 8));      // write first byte
    Wire.write((uint8_t)(value & 0xFF));    // write second byte
    Wire.endTransmission();
}

/*================================================================================*/
/*                                 READ  A REGISTER                               */
/*================================================================================*/
uint16_t regRead(uint8_t address, uint8_t reg) {
    Wire.beginTransmission(address);
    Wire.write(reg);                        // select register
    Wire.endTransmission();
    Wire.requestFrom(address, (uint8_t)2);
    
    
    
    #if EXT_ADC == 1
        uint16_t volatile ADC_raw = (Wire.read() << 8) | Wire.read();
    #elif EXT_ADC == 2
        uint16_t volatile ADC_raw = ( (Wire.read() << 8) | Wire.read() ) >> 4;
    #endif

    return ADC_raw;
}
#endif
