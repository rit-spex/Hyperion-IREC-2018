/*
 * File: IRECHYPERION_Transmit.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LoRa transmitter
 */

/**
 * Initilize the LoRa transmitter.
 */
void init_LoRa(){
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(RFM95_INT,INPUT);
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
      Serial.println("[LoRa] - Init failed");
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
      Serial.println("[LoRa] - setFrequency failed");
  }
  Serial.print("[LoRa] - Set Freq to: "); Serial.println(RF95_FREQ);

  // maximum power
  rf95.setTxPower(23, false);

  Serial.println("[LoRa] - Init success");
}

/**
 * Transmits data via LoRa
 * Args
 *    data: The data to be sent
 *    data_len: The length of the data to be sent
 */
void transmit_data(uint8_t data[], int data_len){
  
  digitalWrite(LED, HIGH);
  // Transmit the data
  rf95.send(data, data_len);
}

