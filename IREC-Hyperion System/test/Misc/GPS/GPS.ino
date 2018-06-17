#include "gps.h"

static const uint32_t VALID_POS_TIMEOUT = 2000;  // ms

void setup() {
  // put your setup code here, to run once:
  gps_setup();
}

void get_pos()
{
  // Get a valid position from the GPS
  int valid_pos = 0;
  uint32_t timeout = millis();
  do {
    if (Serial2.available())
      valid_pos = gps_decode(Serial2.read());
  } while ( (millis() - timeout < VALID_POS_TIMEOUT) && ! valid_pos) ;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  Serial2.begin(9600);

  delay(2000);

  get_pos();
  Serial.println(gps_time);
  Serial.println(gps_aprs_lat);
  Serial.println(gps_aprs_lon);
}
