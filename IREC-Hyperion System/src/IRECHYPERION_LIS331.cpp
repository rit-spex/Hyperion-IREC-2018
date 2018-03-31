/*
 * File: IRECHYPERION_LIS331.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#include <IRECHYPERION.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

int       init_LIS331(); // Init function for LIS331
float     get_lis331_accel_x(); // Acceleration in the x axis m/s
float     get_lis331_accel_y(); // Acceleration in the y axis m/s
float     get_lis331_accel_z(); // Acceleration in the z axis m/s
