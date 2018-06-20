/*
 * GPS
 * File: GPS.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *      Utilities functions for the ublox m8q GPS
 */

#include <SoftwareSerial.h>
#include "GPS_Hyperion.h"
#include "../Pins.h"

static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(COMMS_RX, COMMS_TX);
bool new_data = false;

int init_gps(){

    ss.begin(GPSBaud);
}

void gather_gps_data(){
    
    while (ss.available() > 0) gps.encode(ss.read());
}
