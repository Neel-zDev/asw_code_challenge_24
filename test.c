/**
 * @Title       : Test Module for LIS3MDLTR 3-Axis Magnetometer Sensor Control
 * @Filename    : test.c
 * @Author      : Parveen
 * @Origin Date : 18 April 2024
 * @Version     : 1.0.0
 * @Compiler    : TBD
 * @Target      : TBD
 * 
 * @Brief Test module for validating functions controlling the LIS3MDLTR 3-Axis Magnetometer sensor.
 * 
 * This test module includes functions to test the functionality of various operations
 * related to the LIS3MDLTR 3-Axis Magnetometer sensor. It contains test cases for functions
 * such as retrieving full-scale configuration, setting output data rate, enabling/disabling
 * interrupt pin, and reading axis data.
 */

#include "test.h"
#include <stdio.h>

void test_GetFullScaleConfig() {
    printf("\n-------------------------------------------------\n");
    printf("TEST: test_GetFullScaleConfig\n\n");

    int num_tests = 0;
    int num_passed = 0;

    // SUB-TEST 1: Get full-scale config with valid data
    printf("\tSub-test 1: Get full-scale config with valid data\n");
    uint8_t scale;
    //status_mag_t status = magDriver.GetFullScaleConfig(&scale);
    status_mag_t status = magDriver.GetFullScaleConfig(&scale);
    
    num_tests++;
    if (status == STATUS_MAG_OK && (scale >= 0 && scale <= 3)) {
        printf("\tPASSED - Full-scale configuration: %u\n", scale);
        num_passed++;
    } else {
        printf("\tFAILED - Failed to get full-scale configuration\n");
    }

    // SUB-TEST 2: Get full-scale config with invalid output pointer
    printf("\n\tSub-test 2: Get full-scale config with invalid output pointer\n");
    status = magDriver.GetFullScaleConfig(NULL);
    num_tests++;
    if (status == STATUS_MAG_ERROR) {
        printf("\tPASSED - Failed to get full-scale configuration... invalid output pointer.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Invalid output pointer not detected.\n");
    }

    // SUB-TEST 3: Get bus error
    printf("\n\tSub-test 3: Get bus error\n");
    // Simulate bus error by passing invalid bus address
    status = magDriver.GetFullScaleConfig(&scale);
    num_tests++;
    if (status == STATUS_BUS_ERROR) {
        printf("\tPASSED - Failed to get full-scale configuration: Bus Error.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Bus error not detected.\n");
    }

    printf("\nTotal Tests: %d, Passed: %d, Failed: %d\n", num_tests, num_passed, num_tests - num_passed);
    printf("-------------------------------------------------\n");
}


void test_SetOutputDataRate() {
    printf("\n-------------------------------------------------\n");
    printf("TEST: test_SetOutputDataRate\n\n");

    int num_tests = 0;
    int num_passed = 0;

    // Sub-test 1: Set with valid value
    printf("\tSUB-TEST 1: Set with valid value\n");
    uint8_t valid_output_data_rate = DATARATE_5_Hz; // Example valid output data rate
    status_mag_t status = magDriver.SetOutputDataRate(valid_output_data_rate);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Output data rate set successfully.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Output data rate setting failed.\n");
    }

    // Sub-test 2: Set with invalid value
    printf("\n\tSUB-TEST 2: Set with invalid value\n");
    uint8_t invalid_output_data_rate = 15; // Example invalid output data rate
    status = magDriver.SetOutputDataRate(invalid_output_data_rate);
    num_tests++;
    if (status == STATUS_MAG_ERROR) {
        printf("\tPASSED - Invalid output data rate detected.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Invalid output data rate not detected.\n");
    }

    // Sub-test 3: Set with fast ODR enabled
    printf("\n\tSUB-TEST 3: Set with fast ODR enabled\n");
    uint8_t fast_odr_output_data_rate = DATARATE_1000_Hz; // Example valid output data rate for fast ODR
    status = magDriver.SetOutputDataRate(fast_odr_output_data_rate);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Output data rate set successfully with fast ODR enabled.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Output data rate setting with fast ODR failed.\n");
    }

    // Sub-test 4: Get bus error
    printf("\n\tSUB-TEST 4: Get bus error\n");
    // Simulate bus error by passing invalid bus address
    uint8_t bus_error_output_data_rate = DATARATE_10_Hz; // Example valid output data rate
    status = magDriver.SetOutputDataRate(bus_error_output_data_rate);
    num_tests++;
    if (status == STATUS_BUS_ERROR) {
        printf("\tPASSED - Bus error detected.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Bus error not detected.\n");
    }

    printf("\nTotal Tests: %d, Passed: %d, Failed: %d\n", num_tests, num_passed, num_tests - num_passed);
    printf("-------------------------------------------------\n");
}


void test_GetOutputDataRate(){
    printf("\n-------------------------------------------------\n");
    printf("TEST: test_GetOutputDataRate\n\n");

    int num_tests = 0;
    int num_passed = 0;

    // SUB-TEST 1: Get output data rate in standard mode
    printf("\tSUB-TEST 1: Get output data rate in standard mode\n");
    uint8_t output_data_rate;
    status_mag_t status = magDriver.GetOutputDataRate(&output_data_rate);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Output data rate: %u\n", output_data_rate);
        num_passed++;
    } else {
        printf("\tFAILED - Failed to get output data rate.\n");
    }

    // SUB-TEST 2: Get output data rate in fast mode
    printf("\n\tSUB-TEST 2: Get output data rate in fast mode\n");
    // Simulate fast mode by setting Fast_odr_Value bit
    // Test case can be added to simulate fast mode scenario
    printf("\tSKIPPED - Test case for fast mode not implemented.\n");
    num_tests++;

    // SUB-TEST 3: Get output data rate with invalid output pointer
    printf("\n\tSUB-TEST 3: Get output data rate with invalid output pointer\n");
    status = magDriver.GetOutputDataRate(NULL);
    num_tests++;
    if (status == STATUS_MAG_ERROR) {
        printf("\tPASSED - Failed to get output data rate... invalid output pointer.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Invalid output pointer not detected.\n");
    }

    // SUB-TEST 4: Get bus error
    printf("\n\tSUB-TEST 4: Get bus error\n");
    // Simulate bus error by passing invalid bus address
    uint8_t bus_error_output_data_rate;
    status = magDriver.GetOutputDataRate(&bus_error_output_data_rate);
    num_tests++;
    if (status == STATUS_BUS_ERROR) {
        printf("\tPASSED - Bus error detected.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Bus error not detected.\n");
    }

    printf("\nTotal Tests: %d, Passed: %d, Failed: %d\n", num_tests, num_passed, num_tests - num_passed);
    printf("-------------------------------------------------\n");
}


/*************************************************************
 * Test Function: test_LIS3MDL_EnableDisableIntPin
 * Description: Tests the LIS3MDL_EnableDisableIntPin function.
 *************************************************************/
void test_EnableDisableIntPin() {
    printf("\n-------------------------------------------------\n");
    printf("TEST: test_EnableDisableIntPin\n\n");

    int num_tests = 0;
    int num_passed = 0;

    // SUB-TEST 1: Enable interrupt pin
    printf("\tSUB-TEST 1: Enable interrupt pin\n");
    status_mag_t status = magDriver.EnableDisableIntPin(1);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Interrupt pin enabled.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Failed to enable interrupt pin.\n");
    }

    // SUB-TEST 2: Disable interrupt pin
    printf("\n\tSUB-TEST 2: Disable interrupt pin\n");
    status = magDriver.EnableDisableIntPin(0);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Interrupt pin disabled.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Failed to disable interrupt pin.\n");
    }

    // SUB-TEST 3: Invalid enable/disable flag
    printf("\n\tSUB-TEST 3: Invalid enable/disable flag\n");
    status = magDriver.EnableDisableIntPin(2); // Invalid flag
    num_tests++;
    if (status == STATUS_MAG_ERROR) {
        printf("\tPASSED - Invalid enable/disable flag detected.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Invalid enable/disable flag not detected.\n");
    }

    // SUB-TEST 4: Boundary testing
    printf("\n\tSUB-TEST 4: Boundary testing\n");
    status = magDriver.EnableDisableIntPin(0); // Disable again
    status = magDriver.EnableDisableIntPin(1); // Enable again
    num_tests += 2;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Boundary testing passed.\n");
        num_passed += 2;
    } else {
        printf("\tFAILED - Boundary testing failed.\n");
    }

    printf("\nTotal Tests: %d, Passed: %d, Failed: %d\n", num_tests, num_passed, num_tests - num_passed);
    printf("-------------------------------------------------\n");
}


void test_ReadAxisData() {
    printf("\n-------------------------------------------------\n");
    printf("TEST: test_ReadAxisData\n\n");

    int num_tests = 0;
    int num_passed = 0;

    // SUB-TEST 1: Read X-axis data
    printf("\tSUB-TEST 1: Read X-axis data\n");
    uint16_t x_axis_data;
    status_mag_t status = magDriver.ReadAxisData(LIS3MDL_AXIS_X, &x_axis_data);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - X-axis data: %u\n", x_axis_data);
        num_passed++;
    } else {
        printf("\tFAILED - Failed to read X-axis data.\n");
    }

    // SUB-TEST 2: Read Y-axis data
    printf("\n\tSUB-TEST 2: Read Y-axis data\n");
    uint16_t y_axis_data;
    status = magDriver.ReadAxisData(LIS3MDL_AXIS_Y, &y_axis_data);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Y-axis data: %u\n", y_axis_data);
        num_passed++;
    } else {
        printf("\tFAILED - Failed to read Y-axis data.\n");
    }

    // SUB-TEST 3: Read Z-axis data
    printf("\n\tSUB-TEST 3: Read Z-axis data\n");
    uint16_t z_axis_data;
    status = magDriver.ReadAxisData(LIS3MDL_AXIS_Z, &z_axis_data);
    num_tests++;
    if (status == STATUS_MAG_OK) {
        printf("\tPASSED - Z-axis data: %u\n", z_axis_data);
        num_passed++;
    } else {
        printf("\tFAILED - Failed to read Z-axis data.\n");
    }

    // SUB-TEST 4: Read invalid axis data
    printf("\n\tSUB-TEST 4: Read invalid axis data\n");
    status = magDriver.ReadAxisData(3, &x_axis_data); // Invalid axis type
    num_tests++;
    if (status == STATUS_MAG_ERROR) {
        printf("\tPASSED - Invalid axis data read detected.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Failed to detect invalid axis data read.\n");
    }

    // SUB-TEST 5: Read with NULL pointer
    printf("\n\tSUB-TEST 5: Read with NULL pointer\n");
    status = magDriver.ReadAxisData(LIS3MDL_AXIS_X, NULL); // NULL pointer
    num_tests++;
    if (status == STATUS_MAG_ERROR) {
        printf("\tPASSED - NULL pointer handling detected.\n");
        num_passed++;
    } else {
        printf("\tFAILED - Failed to handle NULL pointer.\n");
    }

    printf("\nTotal Tests: %d, Passed: %d, Failed: %d\n", num_tests, num_passed, num_tests - num_passed);
    printf("-------------------------------------------------\n");
}