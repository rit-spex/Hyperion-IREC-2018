/*
 * File: LSM9DS1_utils.ino
 * Description:
 *     Utilities functions for the SparkFun LSM9DS1 sensor.
 */

/*
 * Starts the LSM9DS1 with correct settings.
 */
int start_LSM9DS1(){
  
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  
  if (!imu.begin()) return 1; // return 1 if error has occured
  // TODO: set error 
}

// request_Gyro() - Requests sensor information, gyroscope data is updated on exit.
void request_Gyro(){
  
  if(imu.gyroAvailable()){
    
    imu.readGyro();
  }
}

// request_Accel() - Requests sensor information, acceleration data is updated on exit.
void request_Accel(){

  if(imu.accelAvailable()){

    imu.readAccel();
  }
}

// request_Mag() - Requests sensor information, Mag data is updated on exit.
void request_Mag(){

  if(imu.magAvailable()){

    imu.readMag();
  }
}

// get_Gyro() -- Grabs raw sensor data, converts into DPS
// Args:
//    - axis: can be either X_AXIS, Y_AXIS, or Z_AXIS
// Return:
//    Sensor data for given axis in DPS
float get_Gyro(lsm9ds1_axis axis) {
  //TODO
  return 0;
}

// get_Accel() -- Grabs raw sensor data, converts into g's
// Args:
//    - axis: can be either X_AXIS, Y_AXIS, or Z_AXIS
// Return:
//    Sensor data for given axis in g's
float get_Accel(lsm9ds1_axis axis) {
  //TODO
  return 0;
}

// get_Mag() -- Grabs raw sensor data, converts into Gauss
// Args:
//    - axis: can be either X_AXIS, Y_AXIS, or Z_AXIS
// Return:
//    Sensor data for given axis in Gauss
float get_Mag(lsm9ds1_axis axis) {
  //TODO
  return 0;
}

// calc_Pitch_Deg() -- Pitch in Degrees
// Returns:
//    Pitch value in degrees
float calc_Pitch_Deg() {
  //TODO
  return 0;
}

// calc_Roll_Deg() -- Roll in Degrees
// Returns:
//    Roll value in degrees
float calc_Roll_Deg(){
  //TODO
  return 0;
}

// calc_Heading_Deg() -- Heading in Degrees
// Returns:
//    Heading value in degrees
float calc_Heading_Deg() {
  //TODO
  return 0;
}

// calc_Pitch_Rad() -- Pitch in Radians
// Returns:
//    Pitch value in Radians
float calc_Pitch_Rad() {
  //TODO
  return 0;
}

// calc_Roll_Deg() -- Roll in Radians
// Returns:
//    Roll value in Radians
float calc_Roll_Rad(){
  //TODO
  return 0;
}

// calc_Heading_Rad() -- Heading in Radians
// Returns:
//    Heading value in Radians
float calc_Heading_Rad() {
  //TODO
  return 0;
}



