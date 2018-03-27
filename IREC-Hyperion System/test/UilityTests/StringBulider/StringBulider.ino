
#include <string.h>

/**
 * Allocate dynamic space for a string and fill the first value with a null terminator.
 * Arg
 *    str_size: the size of the string to be allocated.
 */
char * create_string(int str_size){
  char *result_str = (char*) malloc(str_size + 1);

  if (result_str == NULL) return NULL;

  result_str[0] = '\0';

  return result_str;
}

char* form_test_str(){
  // Create string
  char *data_str = create_string(200);

  if(data_str == NULL) return NULL;

  char data_type[4] = {'0','2',',','\0'};
  char time_str[12] = {'0','0','0','0',',','\0'}; // CHANGE TODO

  strcat(data_str, data_type);
  strcat(data_str, time_str);

  char temp[10];

  float data_array[4] = {1,2,3,4};

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

void setup(){
  Serial.begin(9600);
}

void loop(){
  form_test_str();
}
