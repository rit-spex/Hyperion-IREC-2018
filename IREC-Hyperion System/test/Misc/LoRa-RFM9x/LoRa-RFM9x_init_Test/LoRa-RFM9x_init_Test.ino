#include <RHHardwareSPI1.h>
#include <RH_RF95.h>

#define RF95_FREQ 915.0
#define RFM95_INT       4  
#define RFM95_CS        5 
#define RFM95_RST       6

RH_RF95 rf95(RFM95_CS, RFM95_INT, hardware_spi1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
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
  
}
