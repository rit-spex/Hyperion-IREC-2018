/*
 * File: IRECHYPERION_Transmit.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LoRa transmitter
 */

#include "IRECHYPERION.h"

RH_RF95 rf95(RFM95_CS, RFM95_INT); // Singleton instance of the radio driver

/**
 * Initilize the LoRa transmitter.
 */
int init_LoRa(){

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(RFM95_INT,INPUT);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
      Serial.println("[LoRa] - Init Failure");
      return 1;
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
      Serial.println("[LoRa] - setFrequency failed");
      return 1;
  }
  Serial.print("[LoRa] - Set Freq to: "); Serial.println(RF95_FREQ);

  // maximum power
  rf95.setTxPower(23, false);

  Serial.println("[LoRa] - Init Success");

  return 0;
}

/**
 * Transmits data via LoRa
 * Args
 *    data: The data to be sent
 *    data_len: The length of the data to be sent
 */
void transmit_data(uint8_t data[], int data_len){
  // Transmit the data
  rf95.send(data, data_len);
}

int32_t convert_float_int32(float inputvalue) {
    int32_t f;
    memcpy(&f, &inputvalue, sizeof(f));
    return f;
}
