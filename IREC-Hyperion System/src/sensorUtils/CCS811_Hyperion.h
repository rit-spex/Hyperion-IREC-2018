/*
 * File: CCS811.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the SparkFun CCS811 sensor.
 */

#ifndef CCS811_Hyperion_h
#define CCS811_Hyperion_h

#include "../IRECHYPERION.h"
#include <SparkFunCCS811.h>

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

#define CCS811_NORMAL_MODE 3

extern CCS811 ccs; // CCS811

int       init_CCS811(); // Init function for CCS811
uint16_t  get_CO2(); // returns CO2 in ppm
uint16_t  get_TVOC(); // volatile compounds in ppm

#endif