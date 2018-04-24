/*
 * Routines
 * File: Routines.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *    Routines to be included into the DSQ
 */

#ifndef Routines_Hyperion_h
#define Routines_Hyperion_h


void      R_Default(); // Default routine TODO
void      R_check_deployment(); // Check if deployed
void      R_mission_constraints(); // Check mission constraints
void      R_deploy_parachute(); // Deploy parachute TODO
void      R_deploy_dampers(); // Deploy impact damper TODO
void      R_maintain_dampers(); // Maintain airbag pressure TODO
void      R_seq_LSM9DS1_data(); // log data to buffer
void      R_seq_BME280_data(); // log data to buffer
void      R_seq_CCS811_data(); // Gather, log data to buffer
void	  R_seq_LIS331_data(); // log data to buffer
void      R_seq_Altitude_data();  // log data to buffer
void      R_trans_LSM9DS1(); // Tansmit LSM9DS1 data via the Hyperion Protocol
void      R_trans_BME280(); // Transmit BME280 data via the Hyperion Protocol
void      R_trans_CCS811(); // Transmit CCS811 data via the Hyperion Protocol
void      R_trans_LIS331(); // Transmit LIS331 data via the Hyperion Protocol
void      R_trans_Altitude(); // Transmit Altitude data via the Hyperion Protocol
void      R_trans_Orientation(); // Transmit Orientation data via the Hyperion Protocol TODO
void      R_calc_RateOfClimb(); // Calculate the rate of climb over a more than short time.
void      R_recv_Disarm(); // Disarm the payload TODO
void      R_recv_Arm(); // Arm the payload TODO
void      R_Altitude_data(); // Data routine for the strato logger
void      R_gath_LSM9DS1_data(); // Gather routine for LSM9DS1 sensor
void      R_gath_BME280_data(); // Gather routine for BME280 sensor
void      R_gath_LIS331_data(); // Gather LIS331 routine for LIS331
void      R_Heartbeat(); // Toggle heartbeat led

#endif
