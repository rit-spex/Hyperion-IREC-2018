/// RIT SPEX
/// IREC - Hyperion
/// Thomas Hall
/// Amber Dubill
/// BME 280 Sensor Code

#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

/// Sea Level Temp and Pressure for Launch Location
/// Data is Pressure at Sea Level in hPa
#define SEALEVELPRESSURE_HPA (1023.23)

Adafruit_BME280 bme; // I2C

unsigned long delayTime;

void setup() {
  // Setup code for BME280
  Serial.begin(9600);
  Serial.println(F("BME280 I2C"));

  bool status;
    
  // Check status of BME280
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
    
  Serial.println("-- Test --");
  delayTime = 1000;

  Serial.println();

  delay(100); // let sensor boot up
}

void loop() {
  // Main Loop
  printValues();
  delay(delayTime);
}

void printValues() {
  // Printo out for debug
  //Serial.print("Temperature = ");
  unsigned long time = millis();
  bme.readTemperature();
  Serial.println(millis() - time);
  Serial.print(bme.readTemperature());
  //Serial.println(" *C");

  Serial.print("Pressure = ");
  
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
