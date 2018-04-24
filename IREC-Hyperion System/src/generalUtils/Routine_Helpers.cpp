/*
 * File: Routine_Helpers.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Routine helper functions.
 */

#include "../IRECHYPERION.h"
#include "sensorUtils/LSM9DS1_Hyperion.h"
#include "sensorUtils/BME280_Hyperion.h"
#include "sensorUtils/CCS811_Hyperion.h"
#include "sensorUtils/LIS331_Hyperion.h"
#include "sensorUtils/StratoLogger_Hyperion.h"
#include "../Data_Buffer_Hyperion.h"

/**
 * Helper function for no data buffer strings,
 * Args
 * type: enum to describe the string to be created.
 * Return
 * DEPLOYMENT: "00,[time],DEPLOYMENT"
 * PARACHUTE_DEPLOY: "00,[time],PARACHUTE DEPLOYMENT"
 * DAMPER_DEPLOY: "00,[time],IMPACT DAMPER DEPLOYMENT"
 */
char *form_NoData_str(NoData_Type type){
	// Create string
	char *data_str = create_string(DEFAULT_STR_LEN);

	if(data_str == NULL) return NULL;

	char data_type[4] = {'0','0',',','\0'};
	char time_str[12];

	dtostrf(millis(), 1, 0, time_str);
	strcat(time_str, ",");

	strcat(data_str, data_type);
	strcat(data_str, time_str);

	switch (type) {
		case DEPLOYMENT:
			strcat(data_str, "DEPLOYMENT");
			break;
		case PARACHUTE_DEPLOY:
			strcat(data_str, "PARACHUTE DEPLOYMENT");
			break;
		case DAMPER_DEPLOY:
			strcat(data_str, "IMPACT DAMPER DEPLOYMENT");
			break;
	}

	strcat(data_str, "\n");

	// Reallocate to match the length of the actual string.
	data_str = (char*) realloc(data_str, strlen(data_str)+1);

	if(data_str == NULL) return NULL;

	return data_str;
}

/**
 * Helper function to construct a string of data for the CCS811 sensor to be
 * placed into the data buffer.
 * String created
 *    "03,[Time],[TVOC],[C02]"
 */
char *form_CCS811_str(){
	// Create string
	char *data_str = create_string(DEFAULT_STR_LEN);

	if(data_str == NULL) return NULL;

	char data_type[4] = {'0','3',',','\0'};
	char time_str[12];

	dtostrf(millis(), 1, 0, time_str);
	strcat(time_str, ",");

	strcat(data_str, data_type);
	strcat(data_str, time_str);

	char temp[10];

	uint16_t data_array[2] = {get_TVOC(), get_CO2()};

	for (int i = 0; i < 2; i++){
		itoa(data_array[i], temp, 10);
		strcat(data_str, temp);
		if(i < 1) strcat(data_str, ",");
	}

	strcat(data_str, "\n");

	// Reallocate to match the length of the actual string.
	data_str = (char*) realloc(data_str, strlen(data_str)+1);

	if(data_str == NULL) return NULL;

	return data_str;
}

/**
 * Helper function for the R_seq_LSM9DS1_data routine
 * Creates a string then populates it with data from the LSM9DS1 (imu) sensor.
 * String created:
 *    "01,[Time],[AccelX],[AccelY],[AccelZ],[GyroX],[GyroY],[GyroZ],[MagX],[MagY],[MagZ]"
 */
char *form_LSM9DS1_str(){
	// Create string
	char *data_str = create_string(DEFAULT_STR_LEN);

	if(data_str == NULL) return NULL;

	char data_type[4] = {'0','1',',','\0'};
	char time_str[12];

	dtostrf(millis(), 1, 0, time_str);
	strcat(time_str, ",");

	strcat(data_str, data_type);
	strcat(data_str, time_str);

	char temp[10];

	float data_array[9] = {get_Accel(X_AXIS), get_Accel(Y_AXIS), get_Accel(Z_AXIS),
	get_Gyro(X_AXIS), get_Gyro(Y_AXIS), get_Gyro(Z_AXIS),
	get_Mag(X_AXIS), get_Mag(Y_AXIS), get_Mag(Z_AXIS)};

	for (int i = 0; i < 9; i++){
		dtostrf(data_array[i], 1, 3, temp);
		strcat(data_str, temp);
		if(i < 8) strcat(data_str, ",");
	}

	strcat(data_str, "\n");

	// Reallocate to match the length of the actual string.
	data_str = (char*) realloc(data_str, strlen(data_str)+1);

	if(data_str == NULL) return NULL;

	return data_str;
}

/**
 * Helper funtion for the R_seq_BME280_data routine, creates a data string
 * with data from the BME280 and returns it.
 * String created
 *    "02,[TIME],[TempC],[Pressure],[Hum],[Alt_BME280]"
 */
char* form_BME280_str(){
	// Create string
	char *data_str = create_string(DEFAULT_STR_LEN);

	if(data_str == NULL) return NULL;

	char data_type[4] = {'0','2',',','\0'};
	char time_str[12];

	dtostrf(millis(), 1, 0, time_str);
	strcat(time_str, ",");

	strcat(data_str, data_type);
	strcat(data_str, time_str);

	char temp[10];

	float data_array[4] = {get_Temp(), get_Pressure(), get_Humidity(), get_BME280_Alt()};

	for (int i = 0; i < 4; i++){
		dtostrf(data_array[i], 1, 3, temp);
		strcat(data_str, temp);
		if(i < 3) strcat(data_str, ",");
	}

	strcat(data_str, "\n");

	// Reallocate to match the length of the actual string.
	data_str = (char*) realloc(data_str, strlen(data_str)+1);

	if(data_str == NULL) return NULL;

	return data_str;
}

/**
 * Helper function to construct a string from the R_seq_LIS331_data routine.
 * String created
 *    "04,[TIME],[AccelX],[AccelY],[AccelZ]"
 */
char* form_LIS331_str(){
	// Create string
	char *data_str = create_string(DEFAULT_STR_LEN);

	if(data_str == NULL) return NULL;

	char data_type[4] = {'0','4',',','\0'};
	char time_str[12];

	dtostrf(millis(), 1, 0, time_str);
	strcat(time_str, ",");

	strcat(data_str, data_type);
	strcat(data_str, time_str);

	char temp[10];

	float data_array[3] = {get_lis331_accel_x(), get_lis331_accel_y(), get_lis331_accel_z()};

	for (int i = 0; i < 3; i++){
		dtostrf(data_array[i], 1, 3, temp);
		strcat(data_str, temp);
		if(i < 2) strcat(data_str, ",");
	}

	strcat(data_str, "\n");

	// Reallocate to match the length of the actual string.
	data_str = (char*) realloc(data_str, strlen(data_str)+1);

	if(data_str == NULL) return NULL;

	return data_str;
}

/**
 * Construct a string for the Stratologger data frame.
 * String created:
 *      "05,[TIME],[Altitude]"
 */
char * form_StratoLogger_str(){
	// Create string
	char *data_str = create_string(DEFAULT_STR_LEN);

	if(data_str == NULL) return NULL;

	char data_type[4] = {'0','5',',','\0'};
	char time_str[12];

	dtostrf(millis(), 1, 0, time_str);
	strcat(time_str, ",");

	strcat(data_str, data_type);
	strcat(data_str, time_str);

	char temp[10];

	dtostrf(get_Altitude(), 1, 3, temp);
	strcat(data_str, temp);

	strcat(data_str, "\n");

	// Reallocate to match the length of the actual string.
	data_str = (char*) realloc(data_str, strlen(data_str)+1);

	if(data_str == NULL) return NULL;

	return data_str;
}
