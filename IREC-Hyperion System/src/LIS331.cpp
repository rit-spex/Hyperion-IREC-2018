/*
 * File: LIS331.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#include <IRECHYPERION.h>

// Used for software SPI LIS331
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 35

#define MAX_G 6

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

	pinMode(LIS3DH_CS, OUTPUT);    // CS for SPI
	digitalWrite(LIS3DH_CS, HIGH); // Make CS high
	pinMode(LIS3DH_MOSI, OUTPUT);    // MOSI for SPI
	pinMode(LIS3DH_MISO, INPUT);     // MISO for SPI
	pinMode(LIS3DH_CLK, OUTPUT);    // SCK for SPI
	SPI.begin();

	lis.setSPICSPin(LIS3DH_CS);
	lis.begin(LIS331::USE_SPI);

	Serial.println("[LIS331] Init");

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
