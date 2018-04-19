/*
 * File: StratoLogger.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#include "IRECHYPERION.h"

#define HWSERIAL Serial5

// Altitude in meters
float altitude = -9999; // -9999 init value

bool new_altitude = false; // New data flag

char buff_HWSERIAL[20] = {'\0'};

/**
 * Init function for the stratologger
 */
int init_StratoLogger(){
	HWSERIAL.begin(9600);

	while (!HWSERIAL) {
		delay(1);
	}

	Serial.println("[Strato] Init Success");

	return 0;
}

/**
 * Convert feet to meters
 * Arg
 *    alt_temp: value in feet
 * Returns
 *    val in meters
 */
float convert_feet_meters(float val_temp){
	return val_temp * 0.3048;
}

/**
 * Update altitude with the value stored in the buffer
 */
void alt_update(){
	// Convert buff_HWSERIAL into a singed int
	// Assign to alt varable
	float alt_temp = strtof(buff_HWSERIAL, NULL); // TODO check this

	altitude = convert_feet_meters(alt_temp);

	new_altitude = true;

	Serial.println(buff_HWSERIAL); // TODO remove testing only
	buff_HWSERIAL[0] = '\0'; // Clear buffer
}

/**
 * Grab chars from the UART buffer, ignoring '\r' and trigging a update on a "\n"
 */
int read_HWSERIAL_Strato(){
	int cnt = 0; // Bytes read

	while(HWSERIAL.available()){
		char in = (char) HWSERIAL.read();

		if(in == '\n') alt_update();
		else if (in != '\r'){

			char temp[10] = {in, '\0'};
			strcat(buff_HWSERIAL, temp);
		}

		cnt += 1;
	}

	return cnt;
}

/**
 * Getter function for altitude
 */
float get_Altitude(){
	return altitude;
}

/**
 * Set logged data
 */
void set_old_altitude(){
	new_altitude = false;
}

bool get_new_altitude(){
	return new_altitude;
}

void update_alt_BME280(){
	altitude = get_BME280_Alt();
	new_altitude = true;
}
