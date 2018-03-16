/*
 * File: IRECHYPERION_Data_Buffer.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Main data buffer for the hyperion system.
 */

int buff_size = 0;

// Main data buffer
char *data_buffer[BUFFER_CAP] = {0};

/**
 * Resets the buffer size, previous values in the buffer will be overwritten
 */
void flush_buffer(){
  
  buff_size = 0;
}

/**
 * Add null terminated data string to the buffer, if the buffer is full. Data is not added.
 * Arg
 *    data: null terminated data string
 * Returns
 *    0 on sucess
 *    1 if buffer is full
 */
int add_to_buffer(char data_str[]){
	
	if(buff_size >= BUFFER_CAP) return 1; // Failure
	
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
 * Write the buffer to the SD card
 */
int write_buffer() {
  //return 0;
}

