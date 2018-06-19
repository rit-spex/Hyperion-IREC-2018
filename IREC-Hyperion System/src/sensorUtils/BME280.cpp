/*
 * File: BME280.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the SparkFun BME280 sensor.
 */

#include "../IRECHYPERION.h"
#include "BME280_Hyperion.h"
#include "../generalUtils/Health_Check_Hyperion.h"

float BME280_Temperature = 0;
float BME280_Pressure = 0;
float BME280_Humidity = 0;
float BME280_Altitude = 0;

#define BME280_ADDRESS_HYPER 0x76

Adafruit_BME280 bme;

// Init function for the BME280 sensor
int init_BME280() {

	bool status;

	// Check status
	status = bme.begin(BME280_ADDRESS_HYPER);
	if (!status) {
		send_health_report("BME280 - FAILED TO INITIALIZE\0");
		return 1; // Errors
	}
	return 0; // Correctly started
}

void read_BME280_Sensors(){

	BME280_Temperature = bme.readTemperature();
	BME280_Pressure = bme.readPressure() / 100.0F;
	BME280_Altitude = bme.readAltitude(bme.readAltitude(SEALEVELPRESSURE_HPA));
	BME280_Humidity = bme.readHumidity();
}

// get_Temp() - Produces temperature reading from sensor
// Returns:
//    Temperture in degrees C
float get_Temp() {

	return BME280_Temperature;
}

// get_Pressure() - Produces pressure reading from sensor
// Returns:
//    Pressure in hPa
float get_Pressure() {

	return BME280_Pressure;
}


// get_BME280_Alt() - Approx altitude derived from pressure.
// Returns:
//    Approx alt in meters
float get_BME280_Alt() {

	return BME280_Altitude;
}

// get_Humidity() - produces humidity in percent
// Returns;
//    humidity in percent.
float get_Humidity() {

	return BME280_Humidity;
}
