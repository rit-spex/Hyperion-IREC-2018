/*
 * IREC - Hyperion
 * Organization: RIT Space Exploration
 * ...
 */

// Included libaries
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

#define LED 13

#define DEFAULT_STR_LEN 100

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

//////////////////////
// Global Varables //
////////////////////

DSQ dsq; // Dynamic Scheduling Queue(DSQ)
Adafruit_BME280 bme; // BME280
LSM9DS1 imu; // LSM9DS1
CCS811 ccs(CCS811_ADDR); // CCS811
RH_RF95 rf95(RFM95_CS, RFM95_INT); // Singleton instance of the radio driver

////////////////////////
/// Data structures ///
//////////////////////



//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// IRECHYPERION_LSM9DS1

int       init_LSM9DS1(); // Init function for LSM9DS1 Sensor
void      read_Sensors(); // Read sensors on LSM9DS1
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
void	    R_Default(); // Default routine
void      R_write_buffer(); // Write data in buffer to SD card
void      R_check_deployment(); // Check if deployed
void      R_mission_constraints(); // Check mission constrignts
void      R_deploy_parachute(); // Deploy parachute
void      R_deploy_dampers(); // Deploy impact damper
void      R_maintain_dampers(); // Maintaion airbag pressure
void      R_req_LSM9DS1(); // Request data from LSM9DS1
void      R_seq_LSM9DS1_data(); // Gather, log data to buffer and transmit
void      R_req_BME280(); // Request BME280 data
void      R_seq_BME280_data(); // Gather, log data to buffer and transmit
void      R_req_CCS811(); // Request data from the CCS811
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

void      init_LoRa();
void      transmit_data(uint8_t * data, int data_len);
int32_t   convert_float_int32(float inputvalue);

