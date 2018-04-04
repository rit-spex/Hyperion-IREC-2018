#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SparkFunLSM9DS1.h>

#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW
#define BUFFER_LEN  400

LSM9DS1 imu;

float accelBuffer[BUFFER_LEN][3];
unsigned long timeBuffer[BUFFER_LEN];
const char* filePrefix = "data_";
const char* fileExt = ".log";
char* filename;


void writeData(void);
void storeData(int16_t, int16_t, int16_t);
void createNewFilename(void);

void setup() {
    Serial.begin(9600);
    while (!Serial);

    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.mAddress = LSM9DS1_M;
    imu.settings.device.agAddress = LSM9DS1_AG;
    if (!imu.begin()) {
        Serial.println("Failed to communicate with LSM9DS1.");
        while(1);
    }
    Serial.println("LSM9DS1 is ready to go.");

    if (!SD.begin(4)) {
        Serial.println("SD init failed.");
        while(1);
    }
    Serial.println("initialization done.");
}

void loop() {
    if(imu.accelAvailable()) {
        imu.readAccel();
        storeData(imu.ax, imu.ay, imu.az);
    }
}

void writeData() {
    File dataFile = SD.open(filename, FILE_WRITE);
    for(int i = 0 ; i < BUFFER_LEN ; i++) {
        dataFile.printf("%d,%f,%f,%f\n",timeBuffer[i],accelBuffer[i][0],accelBuffer[i][1],accelBuffer[i][2]);
    }
    dataFile.close();
}

void storeData(int16_t ax, int16_t ay, int16_t az) {
    static int idx = 0;
    timeBuffer[idx] = millis();
    accelBuffer[idx][0] = ax;
    accelBuffer[idx][1] = ay;
    accelBuffer[idx][2] = az;
    idx++;
    if(idx == BUFFER_LEN) {
        idx = 0;
        writeData();
    }
}

void createNewFilename() {
    int count = 0;
    char buffer[12];
    sprintf(buffer,"%s%03d%s",filePrefix,count,fileExt);
    while(SD.exists(buffer)) {
        count++;
        sprintf(buffer,"%s%03d%s",filePrefix,count,fileExt);
    }
    sprintf(filename,"%s%05d%s",filePrefix,count,fileExt);
}
