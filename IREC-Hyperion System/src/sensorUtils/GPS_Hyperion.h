/*
 * GPS
 * File: GPS_Hyperion.h
 * Organization: RIT Space Exploration
 * Desc:
 *      Utilities functions for the ublox m8q GPS
 */

#ifndef GPS_Hyperion_H
#define GPS_Hyperion_H

#include <TinyGPS++.h>

extern TinyGPSPlus gps;

int         init_gps();
void        gather_gps_data();

#endif