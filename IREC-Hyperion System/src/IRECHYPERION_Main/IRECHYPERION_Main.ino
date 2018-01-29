
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
#include <Adafruit_BME280.h>

////////////////////
// Define Macros //
//////////////////

// Pin Macros
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// WARNING CHANGE BASED LOCATION
#define DECLINATION 11.27 // Declination (degrees) in Rochester, NY

/// Sea Level Temp and Pressure for Launch Location
/// Data is Pressure at Sea Level in hPa
#define SEALEVELPRESSURE_HPA (1023.23)

//////////////////////
// Global Varables //
////////////////////

LSM9DS1 imu_LSM9DS1; // LSM9DS1
Adafruit_BME280 bme; // BME280

//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// IRECHYPERION_LSM9DS1

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

// *********************************
// IRECHYPERION_BME280

int       start_BME280(); // Init function for BME280 sensor
float     get_Temp(); // Temp in C
float     get_Pressure(); // Pressure in hPa
float     get_BME280_Alt(); // Approx alt in m
float     get_Humidity(); // Humidity in %



