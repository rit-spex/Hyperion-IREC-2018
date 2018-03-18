#include <RH_RF95.h>
#include <Wire.h>
#include <SPI.h>
#include <IREC_Hyperion_Protocol.h>

// SPI pins
#define RFM95_RST     9   // "A"
#define RFM95_CS      10   // "B"
#define RFM95_INT     4    // "C"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

#define LED 13

RH_RF95 rf95(RFM95_CS, RFM95_INT); // Singleton instance of the radio driver

void setup() {
  // put your setup code here, to run once:
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

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buff[20] = "Hello World";
  float timed = millis();
  rf95.send(buff, 1);
  timed = millis()-timed;
  Serial.print("Transmit Time: "); Serial.println(timed);
}
