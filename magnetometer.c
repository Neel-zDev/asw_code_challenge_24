
/**
 * @Title       : Implementation of LIS3MDLTR 3-Axis Magnetometer Sensor Control
 * @Filename    : magnetometer.c
 * @Author      : Parveen
 * @Origin Date : 18 April 2024
 * @Version     : 1.0.0
 * @Compiler    : TBD
 * @Target      : TBD
 * 
 * @brief Implementation of functions to control the LIS3MDLTR 3-Axis Magnetometer sensor.
 * 
 * This file contains the implementation of functions to interact with the LIS3MDLTR 3-Axis Magnetometer sensor
 * using the I2C communication protocol. It includes functions to retrieve full-scale configuration,
 * set output data rate, read output data rate configuration, enable/disable interrupt pin, and read axis data.
 * 
 * @PRE-CONDITION The sensor must be properly initialized and connected.
 * @PRE-CONDITION There should be a valid I2C communication link established with the sensor.
 * @PRE-CONDITION The sensor's registers for relevant operations via I2C must be accessible.
 * 
 * \NOTE Available Full-Scale Configuration Options:
 *       - ±4 gauss
 *       - ±8 gauss
 *       - ±12 gauss
 *       - ±16 gauss
 * 
 * \NOTE Available Output Data Rate Options:
 *       - 0.625 Hz, 1.25 Hz, 2.5 Hz, 5 Hz, 10 Hz, 20 Hz, 40 Hz, 80 Hz, 155 Hz, 300 Hz, 560 Hz, 1000 Hz
 * 
 * \NOTE The value of the magnetic field is expressed as two’s complement.
 */

#include <stdint.h>
#include <stdlib.h>
#include "i2c.h"
#include "magnetometer.h"


/*************************************************************
* Function : LIS3MDL_getFullScaleConfig()
*//**
* Defined in magnetometer.c
* \brief Retrieves the currently configured full-scale configuration for the LIS3MDLTR 3-Axis Magnetometer sensor.
* 
* This function retrieves the full-scale range setting of the LIS3MDL magnetometer sensor by communicating over the I2C bus..
* The full-scale range determines the maximum magnetic field strength that can be measured before saturation occurs.
* 
*
* @PRE-CONDITION The sensor must be properly initialized and connected.
* @PRE-CONDITION There should be a valid I2C communication link established with the sensor.
* @PRE-CONDITION The sensor's registers for reading the full-scale configuration via I2C must be accessible.
*
* @param[out] pFullScaleRange Pointer to a variable where the retrieved full-scale configuration will be stored.
*
* @return status_mag_t Returns STATUS_MAG_OK if successful, STATUS_BUS_ERROR if there's an I2C bus error,
*                        and STATUS_MAG_ERROR if the retrieved value is invalid.
*
* \NOTE Available Full-Scale Configuration Options:
*       - ±4 gauss
*       - ±8 gauss
*       - ±12 gauss
*       - ±16 gauss
*
* \NOTE Example usage: uint8_t pFullScaleRange; LIS3MDL_getFullScaleConfig(&pFullScaleRange);
*************************************************************/
status_mag_t LIS3MDL_getFullScaleConfig(uint8_t *pFullScaleRange) {
    // Check if the output pointer is NULL
    if (pFullScaleRange == NULL) {
        return STATUS_MAG_ERROR;
    }
    
    uint8_t i2cReadBuffer;
    
    // retrieve the LIS3MDL magnetometer sensor Full Scale Configuration via I2C bus
    if(i2c_read(I2C_BUS_ADDR, REG_CTRL_REG_2, NUM_OF_BYTES_1, &i2cReadBuffer)){       
        // i2c bus has return error
        return STATUS_BUS_ERROR;
    }
    else{
        // Interpret the value read from bit 6 & 7 of CTRL_REG2 register to determine the full-scale configuration
        uint8_t  fullScaleRead = GET_BIT(i2cReadBuffer, REG_CTRL_REG2_BIT_MASK_FS1_FS0, REG_CTRL_REG2_BIT_POS_FS1_FS0);
                
        // Check if the full scale read value is in range
        if ( fullScaleRead < RANGE_4_GAUSS || fullScaleRead > RANGE_16_GAUSS  ) {
            return STATUS_MAG_ERROR;
        }
        else{
            *pFullScaleRange = fullScaleRead;
            return STATUS_MAG_OK;
        
        }   
    }    
}

/*************************************************************
* Function : LIS3MDL_setOutputDataRate()
*//**
* \brief Sets the output data rate of the LIS3MDLTR 3-Axis Magnetometer sensor
* 
* Defined in magneto_meter.c
* 
* This function sets the data rate of the LIS3MDL 3-Axis Magnetometer sensor. The data rate
 * determines the frequency at which the sensor generates new data samples.
*
* @PRE-CONDITION The sensor must be properly initialized and connected.
* @PRE-CONDITION There should be a valid I2C communication link established with the sensor.
* @PRE-CONDITION The sensor's registers for controlling the output data rate via I2C must be accessible.
*
* @param[in] dataRate The desired data rate for the sensor. This should be a value from
*                      the given range. Refer to the LIS3MDL datasheet for specific data rate options and 
*                      their corresponding values.
*
* @return status_mag_t Returns STATUS_MAG_OK if the operation is successful, 
*                        STATUS_BUS_ERROR if there's an I2C bus error,
*                        STATUS_MAG_ERROR if the provided output data rate value is invalid.
*
* \NOTE Available Output Data Rate Options (Refer to the LIS3MDL datasheet for coreponding values):
*       - 0.625 Hz, 1.25 Hz, 2.5 Hz, 5 Hz, 10 Hz, 20 Hz, 40 Hz, 80 Hz, 155 Hz, 300 Hz, 560 Hz, 1000 Hz
*
* \NOTE Example usage: LIS3MDL_setOutputDataRate(DATARATE_100_Hz);
*************************************************************/
status_mag_t LIS3MDL_setOutputDataRate(uint8_t const dataRate) {
    uint8_t currentRegValue;
    uint8_t i2cWriteBuffer;
    uint8_t omValue; // for operative mode (OM) selection
    
    // Read the current value of REG_CTRL_REG_1
    if (i2c_read(I2C_BUS_ADDR, REG_CTRL_REG_1, NUM_OF_BYTES_1, &currentRegValue) != STATUS_OK) {
        // Error reading the current register value
        return STATUS_BUS_ERROR;
    }

    
    if ( dataRate >= DATARATE_0_625_Hz && dataRate <= DATARATE_80_Hz ) {
        // Set output data rate using bits 3-5 (DO2, DO1, DO0) in CTRL_REG1
        i2cWriteBuffer = SET_BIT(dataRate, REG_CTRL_REG1_BIT_MASK_D02_DO1_DO0, REG_CTRL_REG1_BIT_POS_D02_DO1_DO0);
        
    } else if (dataRate > DATARATE_80_Hz && dataRate <= DATARATE_1000_Hz  ) {
        // Set output data rate using bits 6-7 (OM0, OM1) with FAST_ODR bit in CTRL_REG1
        
        // FAST_ODR enables data rates higher than 80 Hz and operative mode bit value represent the modes 
        i2cWriteBuffer = REG_CTRL_REG1_BIT_VAL_FAST_ORD;

        // Determining Operative Mode (OM) value based on Data Rate
        switch (dataRate) {
            case DATARATE_155_Hz:
                omValue = ULTRAHIGH_PERFORMANCE_MODE; // Maximum operative mode value (Ultrahigh-performance mode)
                break;
            case DATARATE_300_Hz:
                omValue = HIGH_PERFORMANCE_MODE; // High-performance mode
                break;
            case DATARATE_560_Hz:
                omValue = MEDIUM_PERFORMANCE_MODE; // Medium-performance mode
                break;
            case DATARATE_1000_Hz:
                omValue = LOW_POWER_MODE; // Low-power mode
                break;
            default:
                break;
        }

        // Inserting the Operative Mode Values
        
        i2cWriteBuffer |= SET_BIT(omValue,REG_CTRL_REG1_BIT_MASK_OM1_OM0,REG_CTRL_REG1_BIT_POS_OM1_OM0 );
    } else {
        // Output data rate value is invalid
        return STATUS_MAG_ERROR;
    }
    
    // Preserve existing settings in control register 1 and only modify the output data rate bits
    i2cWriteBuffer |= currentRegValue;
 
    // Write the data rate configuration to the magnetometer via I2C and return status
    return (i2c_write(I2C_BUS_ADDR, REG_CTRL_REG_1, NUM_OF_BYTES_1, &i2cWriteBuffer) == STATUS_OK) ?
            STATUS_MAG_OK : STATUS_BUS_ERROR;
}

/*************************************************************
 * Function : LIS3MDL_getOutputDataRate()
 *//**
 * \brief Retrieves the current output data rate configuration of the LIS3MDLTR 3-Axis Magnetometer sensor.
 * 
 * This function retrieves the current output data rate configuration of the LIS3MDLTR 3-Axis Magnetometer sensor
 * by communicating over the I2C bus.The data rate determines the frequency at which the sensor
 * generates new data samples.
 *  
 *
 * @param[out] dataRate Pointer to a variable where the retrieved output data rate will be stored.
 *
 * @return status_mag_t Returns STATUS_MAG_OK if successful, STATUS_BUS_ERROR if there's an I2C bus error,
 *                        and STATUS_MAG_ERROR if the retrieved value is invalid.
 *
 * \NOTE Available Output Data Rate Configuration Options (Refer to the LIS3MDL datasheet for coreponding values): 0.625 Hz, 1.25 Hz, 2.5 Hz, 5 Hz, 10 Hz, 20 Hz,
 *       40 Hz, 80 Hz, 155 Hz, 300 Hz, 560 Hz, 1000 Hz
 *
 * \NOTE Example usage: uint8_t dataRate; LIS3MDL_GetOutputDataRate(&dataRate);
 *************************************************************/
status_mag_t LIS3MDL_getOutputDataRate(uint8_t *dataRate) {
    if (dataRate == NULL) {
        return STATUS_MAG_ERROR; // Return error if the pointer is null
    }
    
    uint8_t i2cReadBuffer[1];
    uint8_t omValue, fastOdrValue;

    // Read the Output Data Rate Value from the control register 1
    if (i2c_read(I2C_BUS_ADDR, REG_CTRL_REG_1, NUM_OF_BYTES_1, i2cReadBuffer) != STATUS_OK) {
        return STATUS_BUS_ERROR; // Return bus error if I2C read fails
    }

    // Determine if fast ODR mode is enabled
    fastOdrValue = GET_BIT(i2cReadBuffer[0], REG_CTRL_REG1_BIT_MASK_FAST_ORD,REG_CTRL_REG1_BIT_POS_FAST_ORD );

    
    if (!fastOdrValue) {
        //retrieve output data rate from bits 3-5 (DO2, DO1, DO0) in CTRL_REG1
        *dataRate = GET_BIT(i2cReadBuffer[0], REG_CTRL_REG1_BIT_MASK_D02_DO1_DO0, REG_CTRL_REG1_BIT_POS_D02_DO1_DO0);
         
    } else if (fastOdrValue) { 
        //retrieve output data rate from bits 6-7 (Operative mode bits OM0, OM1) from CTRL_REG1
        omValue = GET_BIT(i2cReadBuffer[0], REG_CTRL_REG1_BIT_MASK_OM1_OM0, REG_CTRL_REG1_BIT_POS_OM1_OM0);
        switch (omValue) {
            case LOW_POWER_MODE:
                *dataRate = DATARATE_1000_Hz;
                break;
            case MEDIUM_PERFORMANCE_MODE:
                *dataRate = DATARATE_560_Hz;
                break;
            case HIGH_PERFORMANCE_MODE:
                *dataRate = DATARATE_300_Hz;
                break;
            case ULTRAHIGH_PERFORMANCE_MODE:
                *dataRate = DATARATE_155_Hz;
                break;
            default:
                return STATUS_MAG_ERROR; // Return error if unknown operative mode
        }
    }
    return STATUS_MAG_OK; 
}

/*************************************************************
 * Function : LIS3MDL_enableDisableIntPin()
 *//**
 * \brief Enables or disables the interrupt pin of the LIS3MDL magnetometer.
 * 
 * This function enables or disables the interrupt pin of the LIS3MDL magnetometer
 * via I2C communication. It just enable/disable the global interrupt of device not the individual axis.
 *
 * Defined in magneto_meter.c
 * 
 * @PRE-CONDITION The magnetometer must be properly initialized and connected.
 * @PRE-CONDITION There should be a valid I2C communication link established with the magnetometer.
 * @PRE-CONDITION The magnetometer's registers for configuring the interrupt pin via I2C must be accessible.
 *
 * @param[in] enableDisable Flag to enable (1) or disable (0) the interrupt pin.
 *
 * @return status_mag_t Returns STATUS_MAG_OK if the operation is successful, STATUS_BUS_ERROR
 *                        if there's an error in I2C communication, and STATUS_MAG_ERROR if the
 *                        function is called with an invalid parameter.
 *
 *
 * \NOTE Example usage: LIS3MDL_EnableDisableIntPin(1); // to enable interrupt pin
 *************************************************************/
 status_mag_t LIS3MDL_enableDisableIntPin(uint8_t enableDisable) { 
    uint8_t i2cReadBuffer;
    uint8_t i2cWriteBuffer;

    // Check if the parameter is valid
    if (enableDisable > 1 || enableDisable < 0) {
        return STATUS_MAG_ERROR; // Return error if called with an invalid parameter
    }

    // Read the current INT_CFG register value
    if (i2c_read(I2C_BUS_ADDR, LIS3MDL_REG_INT_CFG, NUM_OF_BYTES_1, &i2cReadBuffer) != STATUS_OK)
        return STATUS_BUS_ERROR; // Return bus error if I2C read fails

    
    if (enableDisable == 1) {
        // Set IEN bit to enable global interrupt of magnetometer
        i2cWriteBuffer = i2cReadBuffer | LIS3MDL_REG_INT_CFG_MASK_IEN;        
    } else {
        // cllear IEN bit to disable global interrupt of magnetometer
        i2cWriteBuffer = (i2cReadBuffer) & (~LIS3MDL_REG_INT_CFG_MASK_IEN);       
    }

    // Write the modified INT_CFG register value
    if (i2c_write(I2C_BUS_ADDR, LIS3MDL_REG_INT_CFG, NUM_OF_BYTES_1, &i2cWriteBuffer) != STATUS_OK)
        return STATUS_BUS_ERROR; // Return bus error if I2C write fails

    return STATUS_MAG_OK; // Return success after enabling or disabling interrupt pin
}

/*************************************************************
 * Function : LIS3MDL_ReadAxisData()
 *//**
 * \brief Reads the axis data of the LIS3MDL 3-Axis Magnetometer sensor.
 * 
 * Defined in magneto_meter.c 
 * 
 * This function retrieves the magnetic field data measured along a specific axis
 * (X, Y, or Z) from the LIS3MDL magnetometer sensor by specifying the axis type 
 * 
 *  * 
 * @PRE-CONDITION The magnetometer must be properly initialized and connected.
 * @PRE-CONDITION There should be a valid I2C communication link established with the magnetometer.
 * @PRE-CONDITION The magnetometer's registers for reading axis data via I2C must be accessible.
 *
 * @param[in] axisType The type of axis data to be read. This should be a value from
 *                  the AxisOfMagnetometer enumeration.
 * @param[out] axisData Pointer to a variable where the read axis data will be stored.
 *
 * @return status_mag_t Returns STATUS_MAG_OK if the operation is successful, STATUS_BUS_ERROR
 *                        if there's an error in I2C communication, and STATUS_MAG_ERROR if an
 *                        invalid axis type is specified.
 *\NOTE The value of the magnetic field is expressed as two’s complement.
 * 
 *
 * \NOTE Example usage: uint16_t axisData; LIS3MDL_ReadAxisData(LIS3MDL_AXIS_X, &axisData);
 *************************************************************/
status_mag_t LIS3MDL_readAxisData(uint8_t axisType, uint16_t *axisData) {
    if (axisData == NULL) {
        return STATUS_MAG_ERROR; // Return error if NULL pointer
    }    
    uint8_t i2cReadBuffer[2];
    uint8_t axisRegVal;

    // Determine the register address based on the specified axis type
    switch (axisType) {	
        case LIS3MDL_AXIS_X:
            axisRegVal = LIS3MDL_OUT_X_L_REG;
            break;
        case LIS3MDL_AXIS_Y:
            axisRegVal = LIS3MDL_OUT_Y_L_REG;
            break;
        case LIS3MDL_AXIS_Z:
            axisRegVal = LIS3MDL_OUT_Z_L_REG;
            break;
        default:           
            return STATUS_MAG_ERROR; // Return error if an invalid axis type is specified
    }

    // Read output data of the specified axis via i2c
    if (i2c_read(I2C_BUS_ADDR, axisRegVal, NUM_OF_BYTES_2, i2cReadBuffer) != STATUS_OK)
        return STATUS_BUS_ERROR; // Return bus error if I2C read fails

    // Combine the two bytes to form the 16-bit axis data
    *axisData = (uint16_t)((i2cReadBuffer[1] << 8) | i2cReadBuffer[0]);

    return STATUS_MAG_OK; // Return success
}



// Create an instance of the Magnetometer driver structure and initialize function pointers
magnetometer_driver_t magDriver = {
    .GetFullScaleConfig  = LIS3MDL_getFullScaleConfig, /**< Pointer to the function for getting the full-scale configuration */
    .SetOutputDataRate   = LIS3MDL_setOutputDataRate,
    .GetOutputDataRate   = LIS3MDL_getOutputDataRate,
    .EnableDisableIntPin = LIS3MDL_enableDisableIntPin,
    .ReadAxisData        = LIS3MDL_readAxisData
};