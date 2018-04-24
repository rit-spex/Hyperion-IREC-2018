/*
 * File: Transmit.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LoRa transmitter
 */

#ifndef Transmit_Hyperion_h
#define Transmit_Hyperion_h

#include <RH_RF95.h>

extern RH_RF95 rf95; // Singleton instance of the radio driver

int       init_LoRa(); // Initialize the LoRa RFM_9X module
bool      transmit_data(uint8_t * data, int data_len); // Transmit
int32_t   convert_float_int32(float inputvalue); // Converts floats to int32_t
unsigned int get_bandwidth_scaler();

#endif