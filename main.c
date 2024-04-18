/**
 * @Title       : Main Module for Testing LIS3MDLTR 3-Axis Magnetometer Sensor Control
 * @Filename    : main.c
 * @Author      : Parveen
 * @Origin Date : 18 April 2024
 * @Version     : 1.0.0
 * @Compiler    : TBD
 * @Target      : TBD
 * 
 * @brief Main module for testing the functionality of LIS3MDLTR 3-Axis Magnetometer sensor control functions.
 * 
 * This main module contains the entry point for testing the functionality of various operations related to
 * controlling the LIS3MDLTR 3-Axis Magnetometer sensor. It calls the test functions for retrieving full-scale
 * configuration, setting output data rate, enabling/disabling interrupt pin, and reading axis data.
 * 
 * @note This file should be compiled and linked with the test functions defined in the project.
 */

#include <stdio.h>

// Function prototypes for test functions
void test_GetFullScaleConfig(void);
void test_SetOutputDataRate(void);
void test_GetOutputDataRate(void);
void test_EnableDisableIntPin(void);
void test_ReadAxisData(void);

int main()
{
    // Test LIS3MDL_getFullScaleConfig function for LIS3MDLTR 3-Axis Magnetometer sensor
    test_GetFullScaleConfig();
    
    // Test LIS3MDL_setOutputDataRate function for LIS3MDLTR 3-Axis Magnetometer sensor
    test_SetOutputDataRate();
    
     // Test LIS3MDL_getOutputDataRate function for LIS3MDLTR 3-Axis Magnetometer sensor
    test_GetOutputDataRate();
    
     // Test LIS3MDL_enableDisableIntPin function for LIS3MDLTR 3-Axis Magnetometer sensor
    test_EnableDisableIntPin();
    
     // Test LIS3MDL_readAxisData function for LIS3MDLTR 3-Axis Magnetometer sensor
    test_ReadAxisData();
    
    
     printf(" End of LIS3MDLTR 3-Axis Magnetometer sensor testing.\n");
	return 0;
}
