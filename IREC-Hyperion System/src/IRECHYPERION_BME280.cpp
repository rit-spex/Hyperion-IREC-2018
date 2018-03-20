/*
 * File: IRECHYPERION_BME280.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the SparkFun BME280 sensor.
 */

#include "IRECHYPERION.h"

float BME280_Temperature = 0;
float BME280_Pressure = 0;
float BME280_Humidity = 0;
float BME280_Altitude = 0;

Adafruit_BME280 bme;

// Init function for the BME280 sensor
int init_BME280() {
  
  bool status;
    
  // Check status
  status = bme.begin();  
  if (!status) {
      Serial.print("[BME280] - Init failed");
      return 1; // Errors
  }

  Serial.print("[BME280] - Init success");
  return 0; // Correctly started
}


// get_Temp() - Produces temperature reading from sensor
// Returns:
//    Temperture in degrees C
float get_Temp() {

  return bme.readTemperature();
}

// get_Pressure() - Produces pressure reading from sensor
// Returns:
//    Pressure in hPa
float get_Pressure() {

  return bme.readPressure() / 100.0F;
}


// get_BME280_Alt() - Approx altitude derived from pressure.
// Returns:
//    Approx alt in meters
float get_BME280_Alt() {

  return bme.readAltitude(bme.readAltitude(SEALEVELPRESSURE_HPA));
}

// get_Humidity() - produces humidity in percent
// Returns;
//    humidity in percent.
float get_Humidity() {

  return bme.readHumidity();
}


