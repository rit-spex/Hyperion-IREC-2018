/*
 * File: IRECHYPERION_LIS331.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the LIS331 sensor.
 */

#include <IRECHYPERION.h>

// Software SPI
Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

/**
 * Init function for the LIS331
 * Return
 *    0 - Success
 *    1 - Failure
 */
int init_LIS331(){

  if (!lis.begin()) {   // change this to 0x19 for alternative i2c address
    Serial.println("[LIS331] Init Failure");
    return 1;
  }

  Serial.println("[LIS331] Init Success");
  delay(100);

  lis.setRange(LIS3DH_RANGE_16_G);   // 2, 4, 8 or 16 G!

  Serial.print("[LIS331] Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  return 0;
}

/**
 * Read the sensor
 */
void read_LIS331(){
  lis.read();
}

float get_lis331_accel_x(){
  return lis.x_g;
}

float get_lis331_accel_y(){
  return lis.y_g;
}

float get_lis331_accel_z(){
  return lis.z_g;
}
