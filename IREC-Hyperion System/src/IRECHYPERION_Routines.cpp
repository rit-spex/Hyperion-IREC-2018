/*
 * IRECHYPERION_Routines
 * File: IRECHYPERION_Routines.ino
 * Organization: RIT Space Exploration
 * Desc:
 *    Routines to be included into the DSQ
 */

#include "IRECHYPERION.h"

void R_Default(){
	// TODO
}

void R_seq_LSM9DS1_data(){

    // Read data from sensors
    read_Sensors();

    // Create string
    // Pack string with data from the sensors
    char * data_str = form_LSM9DS1_str();
	
    // Insert data into the data buffer
	add_to_buffer(data_str);

    dsq.add_routine(0, 3, R_seq_LSM9DS1_data); 
}

void R_trans_LSM9DS1(){
      // Allocate space for flags
    char flags[4] = {0, 0, 0, 0};
    uint32_t time = 800; // Change

    uint8_t buff[LSM9DS1_FRAME_SIZE+HEADER_SIZE] = {0};
    
    IRECHYPERP::createLSM9DS1Frame(buff, flags, time, 
    convert_float_int32(get_Accel(X_AXIS)), convert_float_int32(get_Accel(Y_AXIS)), convert_float_int32(get_Accel(Z_AXIS)),
    convert_float_int32(get_Gyro(X_AXIS)), convert_float_int32(get_Gyro(Y_AXIS)), convert_float_int32(get_Gyro(Z_AXIS)),
    convert_float_int32(get_Mag(X_AXIS)), convert_float_int32(get_Mag(Y_AXIS)), convert_float_int32(get_Mag(Z_AXIS)));

    float deltat = millis();
    transmit_data(buff, LSM9DS1_FRAME_SIZE+HEADER_SIZE);
    deltat = millis() - deltat;
    // REMOVE
    Serial.print("Transmitted LSM9DS1 data in "); Serial.println(deltat);

    dsq.add_routine(0, 3, R_trans_LSM9DS1); 
}

/**
 * Transmit route for the BME280 data frame
 */
void R_trans_BME280(){
    // Allocate space for flags
    char flags[4] = {0, 0, 0, 0};
    uint32_t time = 800; // Change

    uint8_t buff[BME280_FRAME_SIZE+HEADER_SIZE] = {0};
	  //TODO
    IRECHYPERP::createBME280Frame(buff, flags, time, 
    convert_float_int32(get_Temp()), convert_float_int32(get_Pressure()), convert_float_int32(get_Humidity()), convert_float_int32(get_BME280_Alt()));

    // Transmit data via LoRa
    float deltat = millis();
    transmit_data(buff, BME280_FRAME_SIZE+HEADER_SIZE);
    deltat = millis() - deltat;
    // REMOVE
    Serial.print("Transmitted BME280 data in "); Serial.println(deltat);

    // Add routine back into the DSQ
    dsq.add_routine(0, 20, R_trans_BME280); 
}

