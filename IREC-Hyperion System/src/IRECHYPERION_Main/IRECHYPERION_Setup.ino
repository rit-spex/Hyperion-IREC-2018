/*
 * IRECHYPERION_Setup
 * Organization: RIT Space Exploration
 * Desc:
 *    Code included in setup() will run once during startup.
 */
 
void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    delay(1);
  }
  
  delay(100);
  
  Serial.println("-----Initilizing------");
  // TODO
  // Initilize hardware modules  
  init_LoRa();
  init_BME280();
  
  // Add startup routines into the dsq
  dsq.add_routine(0, 3, R_trans_BME280);
  
  // Add Default routine to the dsq

}
