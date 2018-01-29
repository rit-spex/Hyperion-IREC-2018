/*
 * IREC - Hyperion
 * ...
 * ...
 */

// Included libaries
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <LSM9DS1_Types.h>
#include <LSM9DS1_Registers.h>

////////////////////
// Define Macros //
//////////////////

// Pin Macros
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// WARNING CHANGE BASED LOCATION
#define DECLINATION 11.27 // Declination (degrees) in Rochester, NY

//////////////////////
// Global Varables //
////////////////////

LSM9DS1 imu_LSM9DS1; // LSM9DS1

//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// IRECHYPERION_LSM9DS1_utils

int       start_LSM9DS1(); // Init function for LSM9DS1 Sensor
void      request_Gyro(); // Update sensor values
void      request_Accel(); // Update sensor values
void      request_Mag(); // Update sensor values
float     get_Gyro(lsm9ds1_axis axis); // Return value is in DPS
float     get_Accel(lsm9ds1_axis axis); // Return value is in g's
float     get_Mag(lsm9ds1_axis axis); // Return value is in Gauss
float     calc_Pitch_Deg(); // Return is in Degrees
float     calc_Roll_Deg(); // Return is in Degrees
float     calc_Heading_Deg(); // Return is in Degrees
float     calc_Pitch_Rad(); // Return is in Radians
float     calc_Roll_Rad(); // Return is in Radians
float     calc_Heading_Rad(); // Return is in Radians


