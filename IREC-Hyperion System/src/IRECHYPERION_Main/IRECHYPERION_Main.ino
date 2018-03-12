/*
 * IREC - Hyperion
 * Organization: RIT Space Exploration
 * ...
 */

// Included libaries
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <LSM9DS1_Types.h>
#include <LSM9DS1_Registers.h>
#include <SparkFunCCS811.h>
#include <Adafruit_BME280.h>
#include <DSQ.h>

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

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

#define BUFFER_CAP 500

//////////////////////
// Global Varables //
////////////////////

DSQ dsq; // Dynamic Scheduling Queue(DSQ)
Adafruit_BME280 bme; // BME280
LSM9DS1 imu_LSM9DS1; // LSM9DS1
CCS811 ccs(CCS811_ADDR);; // CCS811

//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// IRECHYPERION_LSM9DS1

int       start_LSM9DS1(); // Init function for LSM9DS1 Sensor
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

// *********************************
// IRECHYPERION_CCS811
// TODO
int       start_CCS811(); // Init function for CCS811
void      read_Algo_Results(); // Reads results
uint16_t  get_CO2(); // returns CO2 in ppm
uint16_t  get_TVOC(); // volitile compounds in ppm

// *********************************
// IRECHYPERION_Routines
// TODO
void      R_write_buffer(); // Write data in buffer to SD card
void      R_check_deployment(); // Check if deployed
void      R_mission_constraints(); // Check mission constrignts
void      R_deploy_parachute(); // Deploy parachute
void      R_deploy_dampers(); // Deploy impact damper
void      R_req_LSM9DS1(); // Request data from LSM9DS1
void      R_seq_LSM9DS1_data(); // Gather, log data to buffer and transmit
void      R_seq_BME280_data(); // Gather, log data to buffer and transmit
void      R_req_CCS811(); // Request data from the CCS811
void      R_seq_CCS811_data(); // Gather, log data to buffer and transmit

// *********************************
// IRECHYPERION_Data_Buffer
//TODO
void      init_buffer(); // Init buffer
void      flush_buffer(); // Clear buffer
int       add_to_buffer(char * data); // Add null terminated string to buffer
int       get_size(); // Get size of buffer
int       write_buffer(); //TODO







