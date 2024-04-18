//magnetometer.h
/**
 * @Title       : Header file for LIS3MDLTR 3-Axis Magnetometer Sensor Control
 * @Filename    : magnetometer.h
 * @Author      : Parveen
 * @Origin Date : 18 April 2024
 * @Version     : 1.0.0
 * @Compiler    : TBD
 * @Target      : TBD
 * 
 * @brief Header file containing interface for controlling the LIS3MDLTR 3-Axis Magnetometer sensor.
 * 
 * This header file contains declarations for functions and constants used to control the LIS3MDLTR 3-Axis Magnetometer sensor
 * using the I2C communication protocol. It includes definitions for status codes, register addresses, data rates,
 * operative modes, and axis types. It also defines an interface structure for magnetometer operations and function prototypes.
 * 
 * @note This file should be included in any source file that requires access to magnetometer functions or constants.
 */


#ifndef MAGNETOMETER_HEADER_H
#define MAGNETOMETER_HEADER_H

#include "stdint.h"

#define REG_CTRL_REG_1 	    (0x20) /* LIS3MDL magnetometer sensor control register 1 address */
#define REG_CTRL_REG_2 	    (0x21) /* LIS3MDL magnetometer sensor control register 2 address */

/**
 * Enumeration defining status codes for LIS3MDL magnetometer functions.
 *
 * Status codes indicate outcomes like retrieving full-scale configuration or setting output data rate function etc.
 */
typedef enum {
    STATUS_MAG_OK,      /* magnetometer Operation completed successfully. */
    STATUS_MAG_ERROR,   /* magnetometer error occurred during the operation at device level (magnetometer). */
    STATUS_BUS_ERROR    /* Error occurred on the I2C bus during communication. */
} status_mag_t;

#define GET_BIT(data, mask, pos) ((data & mask) >> pos)
#define SET_BIT(data, mask, pos) ((data << pos) & mask)

#define I2C_BUS_ADDR        (0x1C)      /* i2c Bus address (Get or update with actual address) */ 
#define NUM_OF_BYTES_2      2           /* number of byte to read/ write in i2c bus */
#define NUM_OF_BYTES_1      1           /* number of byte to read/ write in i2c bus */

/* for full-scale configuration */
#define REG_CTRL_REG2_BIT_MASK_FS1_FS0      0x60  /* Full-scale configuration bits FS0 BIT- 5, FS1 BIT- 6 */
#define REG_CTRL_REG2_BIT_POS_FS1_FS0       5     /* postion to shift for FS bits */

// Full-scale configuration ranges 
#define RANGE_4_GAUSS   0           /* +/- 4g (default value) */
#define RANGE_8_GAUSS   1           /*  +/- 8g */
#define RANGE_12_GAUSS  2           /* +/- 12g */
#define RANGE_16_GAUSS  3           /* +/- 16g */

/* to set/get output data rate*/
// Magnetometer Output Data Rate options (in Hz)
#define DATARATE_0_625_Hz    0   /* 0.625 Hz */
#define DATARATE_1_25_Hz     1   /* 1.25 Hz */
#define DATARATE_2_5_Hz      2   /* 2.5 Hz */
#define DATARATE_5_Hz        3   /* 5 Hz */
#define DATARATE_10_Hz       4   /* 10 Hz */
#define DATARATE_20_Hz       5   /* 20 Hz */
#define DATARATE_40_Hz       6   /* 40 Hz */
#define DATARATE_80_Hz       7   /* 80 Hz */
// Options for higher data rates
#define DATARATE_155_Hz      8   /* 155 Hz */
#define DATARATE_300_Hz      9   /* 300 Hz */
#define DATARATE_560_Hz      10  /* 560 Hz */
#define DATARATE_1000_Hz     11  /* 1000 Hz */

// constants for operative mode values
#define LOW_POWER_MODE           0      /* Low-power mode */
#define MEDIUM_PERFORMANCE_MODE  1      /* Medium-performance mode */
#define HIGH_PERFORMANCE_MODE    2      /* High-performance mode */
#define ULTRAHIGH_PERFORMANCE_MODE 3    /* Ultrahigh-performance mode */

#define REG_CTRL_REG1_BIT_MASK_D02_DO1_DO0     (0x1C)       /* Mask for D02 DO1 DO0 bits to select output data rate */
#define REG_CTRL_REG1_BIT_POS_D02_DO1_DO0      2            /* shift postions to get D02 DO1 DO0 bits */

#define REG_CTRL_REG1_BIT_MASK_OM1_OM0          (0x60)       /* Mask for Operating mode for X and Y axes (OM1 , OM0) */
#define REG_CTRL_REG1_BIT_POS_OM1_OM0           5            /* shift postions to get OM1 OM0 bit bits */  

#define REG_CTRL_REG1_BIT_MASK_FAST_ORD         (0x2)       /* Mask for FAST Out put rate (ODR) bit */
#define REG_CTRL_REG1_BIT_POS_FAST_ORD           1          /* shift postions to get FAST_ODR bit to lsb */

#define REG_CTRL_REG1_BIT_VAL_FAST_ORD           2          /* FAST ODR value */ 

/* for interrupt enable disable */

#define LIS3MDL_REG_INT_CFG                     (0x30)      /* Interrupt configuration register */
#define LIS3MDL_REG_INT_CFG_MASK_IEN            (0x01)      /* MASK for IEN pin to enable/disable interrupt */

/* for reading axis(X,Y,Z) data value */
// Register addresses for axis data output
#define LIS3MDL_OUT_X_L_REG    0x28
#define LIS3MDL_OUT_X_H_REG    0x29
#define LIS3MDL_OUT_Y_L_REG    0x2A
#define LIS3MDL_OUT_Y_H_REG    0x2B
#define LIS3MDL_OUT_Z_L_REG    0x2C
#define LIS3MDL_OUT_Z_H_REG    0x2D

/* Enumeration defining  for LIS3MDL magnetometer axis type */
typedef enum {
    LIS3MDL_AXIS_X = 0,         /* X-axis for magnetometer data */
    LIS3MDL_AXIS_Y = 1,         /* Y-axis for magnetometer data */
    LIS3MDL_AXIS_Z = 2          /* Z-axis for magnetometer data */
} AxisOfMagnetometer;

// Interface structure defining function pointers for magnetometer operations
typedef struct {
    status_mag_t (*GetFullScaleConfig)(uint8_t *pFullScaleRange); // Function pointer for getting the full-scale configuration
    status_mag_t (*SetOutputDataRate)(uint8_t meg_meter_Output_Data_Rate); // Function pointer for setting the output data rate
    status_mag_t (*GetOutputDataRate)(uint8_t *Magnetometer_Data_Rate); // Function pointer for getting the output data rate
    status_mag_t (*EnableDisableIntPin)(uint8_t enable_disable); // Function pointer for enabling or disabling the interrupt pin
    status_mag_t (*ReadAxisData)(uint8_t axis_type, uint16_t *axis_data); // Function pointer for reading axis data
} magnetometer_driver_t;

magnetometer_driver_t magDriver; // declaration of the magnetometer driver instance








#endif  //MAGNETOMETER_HEADER_H