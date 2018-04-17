/*
 * IREC - Hyperion
 * Organization: RIT Space Exploration
 * ...
 */

#ifndef IRECHYPERION
#define IRECHYPERION

// Included libaries
#include <Arduino.h>
#include <string.h>
#include <SD.h>

#include <Wire.h>
#include <SPI.h>

#include <IREC_Hyperion_Protocol.h>
#include <DSQ.h>

#include <LSM9DS1_Types.h>
#include <LSM9DS1_Registers.h>
#include <SparkFunLSM9DS1.h>
#include <SparkFunCCS811.h>
#include <SparkFun_LIS331.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <RH_RF95.h>


////////////////////
// Define Macros //
//////////////////

// File to be created and written
#define FILE_NAME "TEST_01"

#define LED 13 // LED pin

#define DEFAULT_STR_LEN 200

// WARNING CHANGE BASED LOCATION
#define DECLINATION 11.27 // Declination (degrees) in Rochester, NY

/// Sea Level Temp and Pressure for Launch Location
/// Data is Pressure at Sea Level in hPa
#define SEALEVELPRESSURE_HPA (1023.23)

// Maximum buffer capacity
#define BUFFER_CAP 500

#define DEPLOY_SWITCH_01 25 // change
#define DEPLOY_SWITCH_02 26 // change
#define DEPLOY_SWITCH_03 27 // change
#define DEPLOY_SWITCH_04 28 // change

//////////////////////
// Global Varables //
////////////////////

extern DSQ dsq; // Dynamic Scheduling Queue(DSQ)
extern Adafruit_BME280 bme; // BME280
extern LSM9DS1 imu; // LSM9DS1
extern CCS811 ccs; // CCS811
extern RH_RF95 rf95; // Singleton instance of the radio driver
extern LIS331 lis; // LIS331

//////////////
/// Enums ///
////////////

// Used to define "no data" buffer writes
enum NoData_Type{
  DEPLOYMENT,
  PARACHUTE_DEPLOY,
  DAMPER_DEPLOY
};

//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// main

void      switch_to_main();
void      switch_to_done();

// *********************************
// IRECHYPERION_LSM9DS1

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
// IRECHYPERION_LIS331

int       init_LIS331(); // Init function for LIS331
void      read_LIS331(); // Read sensor
float     get_lis331_accel_x(); // Acceleration in the x axis Gees
float     get_lis331_accel_y(); // Acceleration in the y axis Gees
float     get_lis331_accel_z(); // Acceleration in the z axis Gees

// *********************************
// IRECHYPERION_StratoLogger

int       init_StratoLogger(); // Init function for stratologger
float     convert_feet_meters(float val_temp); // Convert feet to meters
int       read_HWSERIAL_Strato(); // Reads the UART buffer
float     get_Altitude(); // Get altitude
void      set_old_altitude();
bool      get_new_altitude();
void      update_alt_BME280();

// *********************************
// IRECHYPERION_Routines
// TODO
void      R_Default(); // Default routine TODO
void      R_check_deployment(); // Check if deployed
void      R_mission_constraints(); // Check mission constrignts TODO
void      R_deploy_parachute(); // Deploy parachute TODO
void      R_deploy_dampers(); // Deploy impact damper TODO
void      R_maintain_dampers(); // Maintaion airbag pressure TODO
void      R_seq_LSM9DS1_data(); // log data to buffer
void      R_seq_BME280_data(); // log data to buffer
void      R_seq_CCS811_data(); // Gather, log data to buffer
void	    R_seq_LIS331_data(); // log data to buffer
void      R_seq_Altitude_data();  // log data to buffer
void      R_trans_LSM9DS1(); // Tansmit LSM9DS1 data via the Hyperion Protocol
void      R_trans_BME280(); // Transmit BME280 data via the Hyperion Protocol
void      R_trans_CCS811(); // Transmit CCS811 data via the Hyperion Protocol
void      R_trans_LIS331(); // Transmit LIS331 data via the Hyperion Protocol TODO
void      R_trans_Altitude(); // Transmit Altitude data via the Hyperion Protocol TODO
void      R_trans_Orientation(); // Transmit Orentation data via the Hyperion Protocol TODO
void      R_calc_RateOfClimb(); // Calculate the rate of climb over a more than short time.
void      R_recv_Disarm(); // Disarm the payload TODO
void      R_recv_Arm(); // Arm the payload TODO
void      R_Altitude_data(); // Data routine for the strato logger
void      R_gath_LSM9DS1_data(); // Gather routine for LSM9DS1 sensor
void      R_gath_BME280_data(); // Gather routine for BME280 sensor
void      R_gath_LIS331_data(); // Gather LIS331 routine for LIS331

// *********************************
// IRECHYPERION_Data_Buffer

int       init_SD();
void      flush_buffer(); // Clear buffer
int       add_to_buffer(char data[]); // Add null terminated string to buffer
int       get_size(); // Get size of buffer
int       write_buffer(); //TODO
char*     create_string(int str_size); // Create a string using dynamic allocation
void      close_file(); // Close file on system suspension.
bool      set_new_file();
void      new_file();

// *********************************
// IRECHYPERION_Transmit

int       init_LoRa(); // Initilize the LoRa RFM_9X module
void      transmit_data(uint8_t * data, int data_len); // Transmit
int32_t   convert_float_int32(float inputvalue); // Converts floats to int32_t

// *********************************
// IRECHYPERION_Routine_Helpers

char*     form_LSM9DS1_str();
char*     form_BME280_str();
char*     form_CCS811_str();
char*     form_NoData_str(NoData_Type type);
char*     form_LIS331_str();
char*     form_StratoLogger_str();

// *********************************
// IRECHYPERION_Mission_Utils

int       init_deploy_pins(); // Initilize the deployment switch pins
uint32_t  deployed_delta(); // Delta time from deployment
void      set_deployment(); // Setter function for deployment;
void      rate_of_climb(); // Rate of climb
float     get_rate_of_climb(); // Getter for rate of climb

#endif
