/*
 * File: IRECHYPERION_Mission_Utils.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions mission events
 */

#include "IRECHYPERION.h"

uint32_t deployment_time = 0;

float height_sample = get_BME280_Alt(); // Use BME280 for now TODO change to Stratologger
uint32_t time_sample = millis();

/**
 * Init function for deployment routines
 */
int init_deploy_pins(){

  pinMode(DEPLOY_SWITCH_01, OUTPUT);
  pinMode(DEPLOY_SWITCH_02, OUTPUT);
  pinMode(DEPLOY_SWITCH_03, OUTPUT);
  pinMode(DEPLOY_SWITCH_04, OUTPUT);

  return 0; // Success
}

/**
 * function to return the time from deployment in millis
 * Return
 *  time from deployment in millis
 */
uint32_t deployed_delta(){

  return millis() - deployment_time;
}

/**
 * Set deployment_time
 */
void set_deployment(){

  char * data_str = form_NoData_str(DEPLOYMENT);

  if (data_str != NULL){
    // Insert data into the data buffer
    add_to_buffer(data_str);
    Serial.println(data_str); // TODO REMOVE this only for testing
  }

  deployment_time = millis();
}

/**
 * Calculate the rate of clib in m/s
 * Return
 *    Positive for increase in altitude
 *    Negitive for decrease in altitude
 */
float rate_of_climb(){

  float delta_time = millis() - time_sample;
  delta_time = delta_time / 1000; // Convert milliseconds to seconds

  float alt_temp = get_BME280_Alt();
  float delta_alt = alt_temp - height_sample;

  time_sample = millis();
  height_sample = alt_temp;

  return delta_alt / delta_time; // result in m/s
}
