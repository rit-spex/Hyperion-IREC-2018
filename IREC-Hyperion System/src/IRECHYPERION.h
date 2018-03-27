/*
 * IREC - Hyperion
 * Organization: RIT Space Exploration
 * ...
 */

#ifndef IRECHYPERION
#define IRECHYPERION

// Included libaries
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>
#include <LSM9DS1_Registers.h>
#include <SparkFunCCS811.h>
#include <Adafruit_BME280.h>
#include <IREC_Hyperion_Protocol.h>
#include <DSQ.h>
#include <RH_RF95.h>
#include <string.h>

////////////////////
// Define Macros //
//////////////////

#define LED 13 // LED pin

#define DEFAULT_STR_LEN 200

// Pin Macros
#define LSM9DS1_M_CS  36 // Can be any digital pin
#define LSM9DS1_AG_CS 35  // Can be any other digital pin

// WARNING CHANGE BASED LOCATION
#define DECLINATION 11.27 // Declination (degrees) in Rochester, NY

/// Sea Level Temp and Pressure for Launch Location
/// Data is Pressure at Sea Level in hPa
#define SEALEVELPRESSURE_HPA (1023.23)

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

// Maximum buffer capacity
#define BUFFER_CAP 500

// SPI pins
#define RFM95_RST     9   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     4    // "C"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

#define CCS811_NORMAL_MODE 3

//////////////////////
// Global Varables //
////////////////////

extern DSQ dsq; // Dynamic Scheduling Queue(DSQ)
extern Adafruit_BME280 bme; // BME280
extern LSM9DS1 imu; // LSM9DS1
extern CCS811 ccs; // CCS811
extern RH_RF95 rf95; // Singleton instance of the radio driver

////////////////////////
/// Data structures ///
//////////////////////



//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// IRECHYPERION_LSM9DS1

int       init_LSM9DS1(); // Init function for LSM9DS1 Sensor
void      read_LSM9DS1_Sensors(); // Read sensors on LSM9DS1
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

int       init_BME280(); // Init function for BME280 sensor
void      read_BME280_Sensors(); // Reads the sensors 
float     get_Temp(); // Temp in C
float     get_Pressure(); // Pressure in hPa
float     get_BME280_Alt(); // Approx alt in m
float     get_Humidity(); // Humidity in %

// *********************************
// IRECHYPERION_CCS811

int       init_CCS811(); // Init function for CCS811
uint16_t  get_CO2(); // returns CO2 in ppm
uint16_t  get_TVOC(); // volitile compounds in ppm

// *********************************
// IRECHYPERION_Routines
// TODO
void      R_Default(); // Default routine
void      R_write_buffer(); // Write data in buffer to SD card
void      R_check_deployment(); // Check if deployed
void      R_mission_constraints(); // Check mission constrignts
void      R_deploy_parachute(); // Deploy parachute
void      R_deploy_dampers(); // Deploy impact damper
void      R_maintain_dampers(); // Maintaion airbag pressure
void      R_seq_LSM9DS1_data(); // Gather, log data to buffer and transmit
void      R_seq_BME280_data(); // Gather, log data to buffer and transmit
void      R_seq_CCS811_data(); // Gather, log data to buffer and transmit
void      R_trans_LSM9DS1(); // Tansmit LSM9DS1 data via the Hyperion Protocol
void      R_trans_BME280(); // Transmit BME280 data via the Hyperion Protocol
void      R_trans_CCS811(); // Transmit CCS811 data via the Hyperion Protocol

// *********************************
// IRECHYPERION_Data_Buffer
//TODO
void      flush_buffer(); // Clear buffer
int       add_to_buffer(char data[]); // Add null terminated string to buffer
int       get_size(); // Get size of buffer
int       write_buffer(); //TODO
char*     create_string(int str_size); // Create a string using dynamic allocation

// *********************************
// IRECHYPERION_Transmit

void      init_LoRa(); // Initilize the LoRa RFM_9X module
void      transmit_data(uint8_t * data, int data_len); // Transmit
int32_t   convert_float_int32(float inputvalue); // Converts floats to int32_t

// *********************************
// IRECHYPERION_Routine_Helpers

char*     form_LSM9DS1_str(); // Construct a string to be added into the data buffer
char*     form_BME280_str(); // Construct a string to be added into the data buffer


#endif
