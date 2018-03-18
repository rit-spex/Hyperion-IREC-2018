/*
 * File: IRECHYPERION_Routine_Helpers.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Routine helper functions.
 */

char *form_LSM9DS1_str(){
  // Create string
  char *data_str = create_string(DEFAULT_STR_LEN);
  
  char data_type[4] = {'0','2',',','\0'};
  char time_str[12] = {'0','0','0','0',',','\0'}; // CHANGE

  strcat(data_str, data_type);
  strcat(data_str, time_str);

  
}

