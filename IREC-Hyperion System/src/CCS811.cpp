/*
 * File: CCS811.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the SparkFun CCS811 sensor.
 */

#include "IRECHYPERION.h"

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

#define CCS811_NORMAL_MODE 3

CCS811 ccs(CCS811_ADDR); // CCS811

/**
 * Startup function for the CCS811
 */
int init_CCS811(){

	CCS811Core::status returnCode = ccs.begin();
	if (returnCode != CCS811Core::SENSOR_SUCCESS) {
		return 1; // Failure
	}

	return 0;
}

/**
 * Getter function for CO2
 */
uint16_t get_CO2(){

	float humidity = get_Humidity();
	float tempC = get_Temp();
	ccs.setEnvironmentalData(humidity,tempC);

	return ccs.getCO2();
}

/**
 * Getter function for TVOC
 */
uint16_t get_TVOC(){

	float humidity = get_Humidity();
	float tempC = get_Temp();
	ccs.setEnvironmentalData(humidity,tempC);

	return ccs.getCO2();
}
