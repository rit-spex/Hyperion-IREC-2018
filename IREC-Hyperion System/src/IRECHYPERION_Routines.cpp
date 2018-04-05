/*
 * IRECHYPERION_Routines
 * File: IRECHYPERION_Routines.ino
 * Organization: RIT Space Exploration
 * Desc:
 *    Routines to be included into the DSQ
 */

#include "IRECHYPERION.h"

#define DEPLOYMENT_ERROR_SPEED -20

void R_Default(){
	// TODO
}

/**
 * Routine to check deployment of the Hyperion payload
 * Cases:
 * open > 2: Deployment of the Hyperion payload
 * 0 < open <= 2: Anomaly case where 1 or 2 switches are open and 3 or 2 switches are still
 * closed.
 * Handling:
 * Case 1: Deployment procedure
 * Case 2: Further checking for deployment
 *    If rate of climb (Negitive) is above DEPLOYMENT_ERROR_SPEED
 *      Deployed
 */
void R_check_deployment(){

  int open_cnt = 0;

  if(digitalRead(DEPLOY_SWITCH_01) == HIGH) open_cnt += 1;
  if(digitalRead(DEPLOY_SWITCH_02) == HIGH) open_cnt += 1;
  if(digitalRead(DEPLOY_SWITCH_03) == HIGH) open_cnt += 1;
  if(digitalRead(DEPLOY_SWITCH_04) == HIGH) open_cnt += 1;

  if(open_cnt > 2){
    // Deployed
    set_deployment(); // Set time deployed
    dsq->add_routine(0, 1, R_mission_constraints);

  } else if (open_cnt == 2){
    // Anomaly case where 2 switches are open and 2 switches are still
    // closed.
    if (rate_of_climb() < DEPLOYMENT_ERROR_SPEED){

      set_deployment(); // Set time deployed
      dsq->add_routine(0, 1, R_mission_constraints);
    } else {

      dsq->add_routine(0, 1, R_check_deployment);
    }
  } else {

    dsq->add_routine(0, 1, R_check_deployment);
  }
}

void R_mission_constraints(){

  // TODO
  // if not parachute
  // Check to see if deployment delta is > 3000 (3 seconds)
  //    if deplyment delta > 3000:
  //        check orentation
  //        if orentation is within acceptable orentation range
  //            set parachute
  //            add deploy R_deploy_parachute() to DSQ
  //
  // if not impact damper
  // Check rate of climb and deployment detla
  //    if (rate of climb range(0 to -15m/s) and parachute) or deplyment delta > 30,000
  //      set impact damper
  //      add R_deploy_dampers() to DSQ
  //
  // If not parachute or not impact damper
  //    add R_mission_constraints to DSQ
  //
}

/**
 * Gathers data then creates a data string to be added into the data buffer
 */
void R_seq_LSM9DS1_data(){

    // Read data from sensors
    read_LSM9DS1_Sensors();

    // Create string
    // Pack string with data from the sensors
    char * data_str = form_LSM9DS1_str();

    if (data_str != NULL){
      // Insert data into the data buffer
      add_to_buffer(data_str);
      Serial.println(data_str); // TODO REMOVE this only for testing
    }

    dsq->add_routine(0, 3, R_seq_LSM9DS1_data);
}

/**
 * Gathers data from the BME280 util then creates a data string to be added
 * into the data buffer.
 */
void R_seq_BME280_data(){

  read_BME280_Sensors();

  // Create string
  // Pack string with data from the sensors
  char * data_str = form_BME280_str();

  if (data_str != NULL){
    // Insert data into the data buffer
    add_to_buffer(data_str);
    Serial.println(data_str); // TODO REMOVE this only for testing
  }

  dsq->add_routine(0, 20, R_seq_BME280_data);
}

/**
 * Transmit routine for the LSM9DS1 data frame.
 */
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

    dsq->add_routine(0, 3, R_trans_LSM9DS1);
}

/**
 * Transmit routine for the BME280 data frame
 */
void R_trans_BME280(){
    // Allocate space for flags
    char flags[4] = {0, 0, 0, 0};
    uint32_t time = 800; // Change

    uint8_t buff[BME280_FRAME_SIZE+HEADER_SIZE] = {0};
	  //TODO
    IRECHYPERP::createBME280Frame(buff, flags, time,
    convert_float_int32(get_Temp()), convert_float_int32(get_Pressure()),
    convert_float_int32(get_Humidity()), convert_float_int32(get_BME280_Alt()));

    // Transmit data via LoRa
    float deltat = millis();
    transmit_data(buff, BME280_FRAME_SIZE+HEADER_SIZE);
    deltat = millis() - deltat;
    // REMOVE
    Serial.print("Transmitted BME280 data in "); Serial.println(deltat);

    // Add routine back into the DSQ
    dsq->add_routine(0, 20, R_trans_BME280);
}
