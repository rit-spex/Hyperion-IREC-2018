/*
 * File: IRECHYPERION_LSM9DS1.ino
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions for the SparkFun LSM9DS1 sensor.
 */

#include "IRECHYPERION.h"

LSM9DS1 imu; 

// Define/setup interupts
// Uses polling currently
//Starts the LSM9DS1 with correct settings.
int init_LSM9DS1(){
  
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_SPI;
  imu.settings.device.mAddress = LSM9DS1_M_CS;
  imu.settings.device.agAddress = LSM9DS1_AG_CS;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    Serial.println("[LSM9DS1] Init Failure");
  }
  Serial.println("[LSM9DS1] Init Success");
}

/**
 * Read values from the sensors on the LSM9DS1
 */
void read_Sensors(){
  
  imu.readMag();
  imu.readAccel();
  imu.readGyro();
}

// get_Gyro() -- Grabs raw sensor data, converts into DPS
// Args:
//    - axis: can be either X_AXIS, Y_AXIS, or Z_AXIS
// Return:
//    Sensor data for given axis in DPS
float get_Gyro(lsm9ds1_axis axis) {
  
  int16_t raw_Data;

  switch(axis){
    case X_AXIS:
      raw_Data = imu.gx;
      break;
    case Y_AXIS:
      raw_Data = imu.gy;
      break;
    case Z_AXIS:
      raw_Data = imu.gz;
      break;
    default:
      return 0; // Did not supply a vaild axis.
  }

  return imu.calcGyro(raw_Data);
}


// get_Accel() -- Grabs raw sensor data, converts into g's
// Args:
//    - axis: can be either X_AXIS, Y_AXIS, or Z_AXIS
// Return:
//    Sensor data for given axis in g's
float get_Accel(lsm9ds1_axis axis) {
  
  int16_t raw_Data;

  switch(axis){
    case X_AXIS:
      raw_Data = imu.ax;
      break;
    case Y_AXIS:
      raw_Data = imu.ay;
      break;
    case Z_AXIS:
      raw_Data = imu.az;
      break;
    default:
      return 0; // Did not supply a vaild axis.
  }
  return imu.calcAccel(raw_Data);
}


// get_Mag() -- Grabs raw sensor data, converts into Gauss
// Args:
//    - axis: can be either X_AXIS, Y_AXIS, or Z_AXIS
// Return:
//    Sensor data for given axis in Gauss
float get_Mag(lsm9ds1_axis axis) {
  
  int16_t raw_Data;

  switch(axis){
    case X_AXIS:
      raw_Data = imu.mx;
      break;
    case Y_AXIS:
      raw_Data = imu.my;
      break;
    case Z_AXIS:
      raw_Data = imu.mz;
      break;
    default:
      return 0; // Did not supply a vaild axis.
  }

  return imu.calcMag(raw_Data);
}


// calc_Pitch_Deg() -- Pitch in Degrees
// Returns:
//    Pitch value in degrees
float calc_Pitch_Deg() {

  return calc_Pitch_Rad() * 180/PI;
}


// calc_Roll_Deg() -- Roll in Degrees
// Returns:
//    Roll value in degrees
float calc_Roll_Deg(){

  return calc_Roll_Rad() * 180/PI;
}


// calc_Heading_Deg() -- Heading in Degrees
// Returns:
//    Heading value in degrees
float calc_Heading_Deg() {

  return calc_Heading_Rad() * 180/PI;
}


// calc_Pitch_Rad() -- Pitch in Radians
// Returns:
//    Pitch value in Radians
float calc_Pitch_Rad() {

  float ax = get_Accel(X_AXIS); // Grab X accel value
  float ay = get_Accel(Y_AXIS); // Grab Y accel value
  float az = get_Accel(Z_AXIS); // Grab Z accel value
  
  return atan2(-ax, sqrt(ay * ay + az * az));
}


// calc_Roll_Rad() -- Roll in Radians
// Returns:
//    Roll value in Radians
float calc_Roll_Rad(){
  
  float ay = get_Accel(Y_AXIS); // Grab Y accel value
  float az = get_Accel(Z_AXIS); // Grab Z accel value
  
  return atan2(ay, az);;
}


// calc_Heading_Rad() -- Heading in Radians
// Returns:
//    Heading value in Radians
float calc_Heading_Rad() {

  float mx = get_Mag(X_AXIS); // Grab X mag value
  float my = get_Mag(Y_AXIS); // Grab Y mag value
  float heading;
  
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;

  return heading;
}

