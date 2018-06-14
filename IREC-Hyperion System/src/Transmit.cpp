/*
 * File: Transmit.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LoRa transmitter
 */

#include "IRECHYPERION.h"
#include "Transmit_Hyperion.h"
#include <RHHardwareSPI1.h>
#include "Pins.h"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

RH_RF95 rf95(COMMS_CS_LORA, LORA_INT, hardware_spi1);

// Start at 30
unsigned int bandwidth_scaler = 30;
bool lora_init = false;

/**
 * Initialize the LoRa transmitter.
 */
int init_LoRa(){

  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, HIGH);
  
  // manual reset
  digitalWrite(LORA_RST, LOW);
  delay(10);
  digitalWrite(LORA_RST, HIGH);
  delay(10);

  SPI1.setMISO(COMMS_MISO);
  SPI1.setMOSI(COMMS_MOSI);
  SPI1.setSCK(COMMS_SCLK); 
  SPI1.begin();

  if (!rf95.init()) {
	  return 1;
  }

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
	  return 1;
  }

  // maximum power
  rf95.setTxPower(23, false);

  Serial.println("THE LORA WORKS MOTHA FUCKA");
  lora_init = true;
  return 0;
}


/**
 * Transmits data via LoRa
 * Args
 *    data: The data to be sent
 *    data_len: The length of the data to be sent
 */
bool transmit_data(uint8_t data[], int data_len){
  // Transmit the data
  // TODO Work on transmit scaling 

  if(!lora_init) return false;

  bool res = rf95.send(data, data_len);

  // Increase scaler value if can transmit, else wait longer
  if(res){
    bandwidth_scaler -= 1;
  } else {
    bandwidth_scaler += 1;
  }
  
  return res;
}

/**
 * Getter function for bandwidth_scaler
 */
unsigned int get_bandwidth_scaler(){
  return bandwidth_scaler;
}

int32_t convert_float_int32(float inputvalue) {
	int32_t f;
	memcpy(&f, &inputvalue, sizeof(f));
	return f;
}
