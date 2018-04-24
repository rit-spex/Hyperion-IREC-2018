/*
 * File: BME280.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the SparkFun BME280 sensor.
 */


#ifndef BME280_Hyperion_h
#define BME280_Hyperion_h

#include "../IRECHYPERION.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/// Sea Level Temp and Pressure for Launch Location
/// Data is Pressure at Sea Level in hPa
#define SEALEVELPRESSURE_HPA (1023.23)

extern Adafruit_BME280 bme; // BME280

int       init_BME280(); // Init function for BME280 sensor
void      read_BME280_Sensors(); // Reads the sensors
float     get_Temp(); // Temp in C
float     get_Pressure(); // Pressure in hPa
float     get_BME280_Alt(); // Approx alt in m
float     get_Humidity(); // Humidity in %

#endif