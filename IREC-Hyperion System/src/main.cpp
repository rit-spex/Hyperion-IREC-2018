/*
 * main.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *    Code included in loop() will run execute in a loop.
 */

#include "IRECHYPERION.h"

#define DSQ_MAIN_CAP 250
#define DSQ_SUSPENDED_CAP 50

// Main DSQ to be used when armed
DSQ main_DSQ(DSQ_MAIN_CAP);
// Suspended DSQ used when idle
DSQ sus_DSQ(DSQ_SUSPENDED_CAP);

// Current DSQ in use
DSQ *dsq = &main_DSQ; // Dynamic Scheduling Queue(DSQ)

void fill_mainDSQ_startup(){
  // Add Default routine to the dsq
  main_DSQ.set_default(1, R_Default);

  // Add startup routines into the dsq
  main_DSQ.add_routine(0, 3, R_trans_LSM9DS1);
  main_DSQ.add_routine(0, 20, R_trans_BME280);
  main_DSQ.add_routine(0, 3, R_seq_LSM9DS1_data);
  main_DSQ.add_routine(0, 20, R_seq_BME280_data);
}

void fill_susDSQ_startup(){

  // Add Default routine to the dsq
  sus_DSQ.set_default(1, R_Default);

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

  fill_mainDSQ_startup();
  fill_susDSQ_startup();
}

void loop() {
  // Execute routine placed into the DSQ
  dsq->execute();
}
