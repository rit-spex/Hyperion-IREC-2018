/*
 * File: StratoLogger.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#ifndef StratoLogger_Hyperion_h
#define StratoLogger_Hyperion_h

int       init_StratoLogger(); // Init function for stratologger
float     convert_feet_meters(float val_temp); // Convert feet to meters
int       read_HWSERIAL_Strato(); // Reads the UART buffer
float     get_Altitude(); // Get altitude
void      set_old_altitude();
bool      get_new_altitude();
void      update_alt_BME280();

#endif