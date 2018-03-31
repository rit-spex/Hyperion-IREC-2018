/*
 * File: IRECHYPERION_Mission_Utils.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions mission events
 */

#include "IRECHYPERION.h"

uint32_t deployment_time = 0;

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

  deployment_time = millis();
}
