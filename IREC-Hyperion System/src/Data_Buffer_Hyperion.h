/*
 * File: Data_Buffer.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Main data buffer for the hyperion system.
 * Details
 *     Strings are stored into the buffer in the following format:
 *     [Data_Type],[Time_from_Startup_Millis],[Data],[Data]...'\n'
 */

#ifndef Data_Buffer_Hyperion_h
#define Data_Buffer_Hyperion_h

bool      init_SD();
void      flush_buffer(); // Clear buffer
int       add_to_buffer(char data[]); // Add null terminated string to buffer
int       get_size(); // Get size of buffer
int       write_buffer(); //TODO
char*     create_string(int str_size); // Create a string using dynamic allocation
void      close_file(); // Close file on system suspension.
bool      set_new_file();
void      new_file();

#endif