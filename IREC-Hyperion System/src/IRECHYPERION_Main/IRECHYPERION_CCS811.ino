/******************************************************************************
  BasicReadings.ino

  Marshall Taylor @ SparkFun Electronics
  Nathan Seidle @ SparkFun Electronics

  April 4, 2017

  https://github.com/sparkfun/CCS811_Air_Quality_Breakout
  https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library

  Read the TVOC and CO2 values from the SparkFun CSS811 breakout board

  This is the simplest example.  It throws away most error information and
  runs at the default 1 sample per second.

  A new sensor requires at 48-burn in. Once burned in a sensor requires
  20 minutes of run in before readings are considered good.

  Hardware Connections (Breakoutboard to Arduino):
  3.3V to 3.3V pin
  GND to GND pin
  SDA to A4
  SCL to A5

  Resources:
  Uses Wire.h for i2c operation

  Development environment specifics:
  Arduino IDE 1.8.1

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/
#include "Arduino.h"
#include "SparkFunCCS811.h"
#include "SparkFunBME280.h"

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

#define NORMAL_MODE 3

void bme_setup(void);


CCS811 gasSensor(CCS811_ADDR);
BME280 envSensor;

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("CCS811 & BME280 Basic Example");

  // Setup CCS811
  CCS811Core::status returnCode = gasSensor.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS) {
    Serial.println(".begin() returned with an error.");
    while (1); //Hang if there was a problem.
  }

  // Setup BME280
  bme_setup();
  delay(10);
  envSensor.begin();

}

void loop() {
  //Check to see if data is ready with .dataAvailable()
  if (gasSensor.dataAvailable()) {
    
    //If so, have the sensor read and calculate the results.
    //Get them later
    gasSensor.readAlgorithmResults();

    Serial.print("CO2[");
    //Returns calculated CO2 reading
    Serial.print(gasSensor.getCO2());
    Serial.print("] tVOC[");
    //Returns calculated TVOC reading
    Serial.print(gasSensor.getTVOC());
    Serial.print("] millis[");
    //Simply the time since program start
    Serial.print(millis());
    Serial.print("]");
    Serial.println();
  }
  float humidity = envSensor.readFloatHumidity();
  float tempC = envSensor.readTempC();
  gasSensor.setEnvironmentalData(humidity,tempC);
  delay(250);

  Serial.print("Temperature: ");
	Serial.print(envSensor.readTempC(), 2);
	Serial.println(" degrees C");

	Serial.print("Temperature: ");
	Serial.print(envSensor.readTempF(), 2);
	Serial.println(" degrees F");

	Serial.print("Pressure: ");
	Serial.print(envSensor.readFloatPressure(), 2);
	Serial.println(" Pa");

	Serial.print("Altitude: ");
	Serial.print(envSensor.readFloatAltitudeMeters(), 2);
	Serial.println("m");

	Serial.print("Altitude: ");
	Serial.print(envSensor.readFloatAltitudeFeet(), 2);
	Serial.println("ft");	

	Serial.print("%RH: ");
	Serial.print(envSensor.readFloatHumidity(), 2);
	Serial.println(" %");
	
	Serial.println();

  delay(2000);
}

void bme_setup() {
  envSensor.settings.commInterface = I2C_MODE;
	envSensor.settings.I2CAddress = 0x77;
  envSensor.settings.runMode = NORMAL_MODE;
  envSensor.settings.tStandby = 0;
	envSensor.settings.filter = 0;
	envSensor.settings.tempOverSample = 5;
  envSensor.settings.pressOverSample = 5;
	envSensor.settings.humidOverSample = 5;
}