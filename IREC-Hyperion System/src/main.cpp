/*
 * main.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *    Code included in loop() will run execute in a loop.
 */

#include "IRECHYPERION.h"

DSQ dsq; // Dynamic Scheduling Queue(DSQ)

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    delay(1);
  }

  delay(100);

  Serial.println("-----Initializing Hyperion------");
  // TODO
  // Initilize hardware modules
  init_LoRa();
  init_BME280();
  init_LSM9DS1();
  init_CCS811();

  // Add Default routine to the dsq
  dsq.set_default(1, R_Default);

  // Add startup routines into the dsq
  dsq.add_routine(0, 20, R_trans_BME280);
  dsq.add_routine(0, 3, R_trans_LSM9DS1);
  dsq.add_routine(0, 3, R_seq_LSM9DS1_data);
}

void loop() {
  // Execute routine placed into the DSQ
  dsq.execute();
}
