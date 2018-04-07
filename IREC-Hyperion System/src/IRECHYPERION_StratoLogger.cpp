/*
 * File: IRECHYPERION_StratoLogger.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#include "IRECHYPERION.h"

#define HWSERIAL Serial1

char buff_HWSERIAL[20] = {'\0'};

int init_StratoLogger(){
  HWSERIAL.begin(9600);

  while (!HWSERIAL) {
    delay(1);
  }

  Serial.println("[Strato] Init Success");

  return 0;
}

void alt_update(){
  // Convert buff_HWSERIAL into a singed int
  // Assign to alt varable

  Serial.println(buff_HWSERIAL); // TODO remove testing only 
  buff_HWSERIAL[0] = '\0'; // Clear buffer
}

void read_HWSERIAL(){
  while(HWSERIAL.available()){
    char in = (char) HWSERIAL.read();

    if(in == '\n') alt_update();
    else if (in != '\r'){

      char temp[10] = {in, '\0'};
      strcat(buff_HWSERIAL, temp);
    }
  }
}
