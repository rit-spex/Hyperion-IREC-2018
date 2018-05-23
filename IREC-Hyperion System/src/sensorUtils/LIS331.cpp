/*
 * File: LIS331.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#include "LIS331_Hyperion.h"

// Software SPI
LIS331 lis;

int16_t x, y, z;

/**
 * Init function for the LIS331
 * Return
 *    0 - Success
 *    1 - Failure
 */
int init_LIS331(){

	pinMode(SENSOR_CS_LIS, OUTPUT);    // CS for SPI
	digitalWrite(SENSOR_CS_LIS, HIGH); // Make CS high
	pinMode(SENSORS_MOSI, OUTPUT);    // MOSI for SPI
	pinMode(SESNORS_MISO, INPUT);     // MISO for SPI
	pinMode(SENSORS_SCLK, OUTPUT);    // SCK for SPI
	SPI.begin();

	lis.setSPICSPin(SENSOR_CS_LIS);
	lis.begin(LIS331::USE_SPI);

	return 0;
}

/**
 * Read the sensor
 */
void read_LIS331(){
	lis.readAxes(x, y, z);
}

float get_lis331_accel_x(){
	return lis.convertToG(MAX_G,x);
}

float get_lis331_accel_y(){
	return lis.convertToG(MAX_G,y);
}

float get_lis331_accel_z(){
	return lis.convertToG(MAX_G,z);
}
