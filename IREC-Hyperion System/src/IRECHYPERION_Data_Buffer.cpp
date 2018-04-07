/*
 * File: IRECHYPERION_Data_Buffer.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Main data buffer for the hyperion system.
 * Details
 *     Strings are stored into the buffer in the following format:
 *     [Data_Type],[Time_from_Startup_Millis],[Data],[Data]...'\n'
 */

#include "IRECHYPERION.h"

size_t buff_size = 0;

File data_file;

// Main data buffer
char *data_buffer[BUFFER_CAP] = {0};

int init_SD(){

  SD.begin();
  data_file = SD.open(FILE_NAME, FILE_WRITE);

  return 0;
}

void close_file(){

  data_file.close();
}

/**
 * Resets the buffer size, previous values in the buffer will be overwritten
 */
void flush_buffer(){

  data_file.flush(); // Force write to file

  for(size_t i = 0; i < buff_size; i++){
    free(data_buffer[i]);
  }

  buff_size = 0;
}

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

/**
 * Add null terminated data string to the buffer, if the buffer is full. Data is not added.
 * Arg
 *    data: null terminated data string
 * Returns
 *    0 on sucess
 *    1 if buffer is full
 */
int add_to_buffer(char * data_str){

	if(buff_size >= BUFFER_CAP) write_buffer();

	data_buffer[buff_size] = data_str;
	buff_size += 1;

	return 0; // Success
}

/**
 * Getter for buff_size
 */
int get_size() {
  return buff_size;
}

/**
 * Write the buffer to the SD card, then flush the buffer
 */
int write_buffer() {

  if (data_file){
    for (size_t i = 0; i < buff_size; i++) {
      data_file.write(data_buffer[i]);
    }
  }

  flush_buffer();
  return 0;
}
