/*
 * File: IRECHYPERION_Routine_Helpers.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Routine helper functions.
 */

/**
 * Helper function for the R_seq_LSM9DS1_data routine
 * Creates a string then populates it with data from the LSM9DS1 (imu) sensor.
 */
char *form_LSM9DS1_str(){
  // Create string
  char *data_str = create_string(DEFAULT_STR_LEN);
  
  char data_type[4] = {'0','2',',','\0'};
  char time_str[12] = {'0','0','0','0',',','\0'}; // CHANGE

  strcat(data_str, data_type);
  strcat(data_str, time_str);

  char temp[10];
  
  float data_array[9] = {get_Accel(X_AXIS), get_Accel(Y_AXIS), get_Accel(Z_AXIS),
  get_Gyro(X_AXIS), get_Gyro(Y_AXIS), get_Gyro(Z_AXIS),
  get_Mag(X_AXIS), get_Mag(Y_AXIS), get_Mag(Z_AXIS)};
  
  for (int i = 0; i < 9; i++){
  	dtostrf(data_array[i], 4, 3, temp);
	  strcat(data_str, ",\0");
	  strcat(data_str, temp);
  }

  // Reallocate to match the length of the actual string.
  data_str = (char*) realloc(data_str, strlen(data_str)+1);
  
  Serial.println(data_str); // REMOVE this only for testing 
  
  return data_str;
}

