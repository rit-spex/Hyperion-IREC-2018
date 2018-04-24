/*
 * File: LIS331.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#ifndef LIS331_Hyperion_h
#define LIS331_Hyperion_h

#include "../IRECHYPERION.h"
#include <SparkFun_LIS331.h>

// Used for software SPI LIS331
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 35
#define MAX_G 6

extern LIS331 lis; // LIS331

int       init_LIS331(); // Init function for LIS331
void      read_LIS331(); // Read sensor
float     get_lis331_accel_x(); // Acceleration in the x axis Gees
float     get_lis331_accel_y(); // Acceleration in the y axis Gees
float     get_lis331_accel_z(); // Acceleration in the z axis Gees

#endif
