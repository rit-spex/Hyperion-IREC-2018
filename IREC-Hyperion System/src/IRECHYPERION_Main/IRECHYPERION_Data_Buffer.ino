/*
 * File: IRECHYPERION_Data_Buffer.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Main data buffer for the hyperion system.
 */

int buff_size = 0;

// Main data buffer
char *data_buffer[BUFFER_CAP];

/**
 * Init buffer 
 */
void init_buffer(){

}

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
int add_to_buffer(char * data){
  
}

int       get_size(); // Get size of buffer
int       write_buffer(); //TODO
