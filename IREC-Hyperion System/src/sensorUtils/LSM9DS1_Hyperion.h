/*
 * File: LSM9DS1_Hyperion.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Header file for LSM9DS1 utils.
 */

#ifndef LSM9DS1_Hyperion_h
#define LSM9DS1_Hyperion_h

#include "../IRECHYPERION.h"
#include <LSM9DS1_Types.h>
#include <LSM9DS1_Registers.h>
#include <SparkFunLSM9DS1.h>
#include "Pins.h"

// WARNING CHANGE BASED LOCATION
#define DECLINATION 11.27 // Declination (degrees) in Rochester, NY

extern LSM9DS1 imu; // LSM9DS1

int       init_LSM9DS1(); // Init function for LSM9DS1 Sensor
void      read_LSM9DS1_Sensors(); // Read sensors on LSM9DS1
float     get_Gyro(lsm9ds1_axis axis); // Return value is in DPS
float     get_Accel(lsm9ds1_axis axis); // Return value is in g's
float     get_Mag(lsm9ds1_axis axis); // Return value is in Gauss
float     calc_Pitch_Deg(); // Return is in Degrees
float     calc_Roll_Deg(); // Return is in Degrees
float     calc_Yaw_Deg(); // Return in Degrees
float     calc_Heading_Deg(); // Return is in Degrees
float     calc_Pitch_Rad(); // Return is in Radians
float     calc_Roll_Rad(); // Return is in Radians
float     calc_Yaw_Rad(); // Return is in Radians
float     calc_Heading_Rad(); // Return is in Radians

#endif
