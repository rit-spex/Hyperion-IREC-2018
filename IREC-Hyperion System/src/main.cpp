/*
 * main.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *    Code included in loop() will run execute in a loop.
 */

#include "IRECHYPERION.h"

#define DSQ_MAIN_CAP 250
#define DSQ_SUSPENDED_CAP 50

// Current DSQ in use
DSQ dsq(DSQ_MAIN_CAP);// Dynamic Scheduling Queue(DSQ)

/**
 * add startup routines to the dsq for main phase
 */
void fill_main_startup(){
  //TODO
  // Add startup routines into the dsq
  dsq.add_routine(0, 3, R_trans_LSM9DS1);
  dsq.add_routine(0, 20, R_trans_BME280);
  dsq.add_routine(0, 30, R_trans_CCS811);
  dsq.add_routine(0, 3, R_seq_LSM9DS1_data);
  dsq.add_routine(0, 20, R_seq_BME280_data);
  dsq.add_routine(0, 30, R_seq_CCS811_data);
}

/**
 * add startup routines to the dsq for suspended phase
 */
void fill_sus_startup(){
  //TODO
  dsq.add_routine(0, 3, R_seq_LSM9DS1_data);
}

/**
 * add startup routines to the dsq for done phase
 */
void fill_done_startup(){
  //TODO
}

/**
 * Switches to the main flight phase, ARMED
 */
void switch_to_main(){

  dsq.clear();
  fill_main_startup();
}

/**
 * Switches to done phase after mission success
 */
void switch_to_done(){

  dsq.clear();
  fill_sus_startup();
}

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }

  delay(100);

  Serial.println("-----Initializing Hyperion-----");
  // TODO
  // Initilize hardware modules
  init_LoRa();
  init_BME280();
  init_LSM9DS1();
  init_CCS811();
  init_SD();
  //init_deploy_pins(); // Initilize pins which deployment switches are attached.

  // Add Default routine to the dsq
  dsq.set_default(1, R_Default);

  fill_sus_startup();
}

void loop() {
  // Execute routine placed into the DSQ
  dsq.execute();
}
