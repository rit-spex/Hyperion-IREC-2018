/*
 * File: IRECHYPERION_Routine_Helpers.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Routine helper functions.
 */

#include "IRECHYPERION.h"

/**
 * Helper function for the R_seq_LSM9DS1_data routine
 * Creates a string then populates it with data from the LSM9DS1 (imu) sensor.
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

  Serial.println(data_str); // REMOVE this only for testing

  return data_str;
}

/**
 * Helper funtion for the R_seq_BME280_data routine, creates a data string
 * with data from the BME280 and returns it.
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

  Serial.println(data_str); // REMOVE this only for testing

  return data_str;
}
