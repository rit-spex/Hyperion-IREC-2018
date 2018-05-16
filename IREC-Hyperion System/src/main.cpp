/*
 * main.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *    Main entry point into the Hyperion system.
 */

#include "IRECHYPERION.h"
#include <Arduino.h>
#include "sensorUtils/LSM9DS1_Hyperion.h"
#include "sensorUtils/BME280_Hyperion.h"
#include "sensorUtils/CCS811_Hyperion.h"
#include "sensorUtils/LIS331_Hyperion.h"
#include "sensorUtils/StratoLogger_Hyperion.h"
#include "generalUtils/Mission_Utils_Hyperion.h"
#include "Routines_Hyperion.h"
#include "Transmit_Hyperion.h"
#include "Data_Buffer_Hyperion.h"

#define DSQ_MAIN_CAP 100

// Current DSQ in use
DSQ dsq(DSQ_MAIN_CAP);// Dynamic Scheduling Queue (DSQ)

/**
 * add startup routines to the dsq for main phase
 */
void fill_main_startup(){
	//TODO
	// Add startup routines into the dsq
	//dsq.add_routine(0, 3, R_trans_LSM9DS1);
	//dsq.add_routine(0, 20, R_trans_BME280);
	//dsq.add_routine(0, 30, R_trans_CCS811);
	//dsq.add_routine(0, 10, R_trans_LIS331);
	//dsq.add_routine(0, 10, R_trans_Altitude);
	dsq.add_routine(0, 3, R_seq_LSM9DS1_data);
	dsq.add_routine(0, 20, R_seq_BME280_data);
	//dsq.add_routine(0, 30, R_seq_CCS811_data);
	dsq.add_routine(0, 3, R_seq_LIS331_data);
	dsq.add_routine(0, 100, R_calc_RateOfClimb);
	dsq.add_routine(0, 10, R_Altitude_data);
	dsq.add_routine(0, 10, R_seq_Altitude_data);
	dsq.add_routine(0, 1, R_check_deployment);
	dsq.add_routine(0, 3, R_gath_LSM9DS1_data);
	dsq.add_routine(0, 20, R_gath_BME280_data);
	dsq.add_routine(0, 3, R_gath_LIS331_data);
	dsq.add_routine(0, 100, R_Heartbeat);
}

/**
 * add startup routines to the dsq for suspended phase
 */
void fill_safe_startup(){
	
	dsq.add_routine(0, 100, R_calc_RateOfClimb);
	dsq.add_routine(0, 10, R_Altitude_data);
	dsq.add_routine(0, 3, R_gath_LSM9DS1_data);
	dsq.add_routine(0, 20, R_gath_BME280_data);
	dsq.add_routine(0, 3, R_gath_LIS331_data);
	//dsq.add_routine(0, 3, R_trans_LSM9DS1);
	//dsq.add_routine(0, 20, R_trans_BME280);
	//dsq.add_routine(0, 30, R_trans_CCS811);
	//dsq.add_routine(0, 10, R_trans_LIS331);
	//dsq.add_routine(0, 10, R_trans_Altitude);
	dsq.add_routine(0, 100, R_Heartbeat);
}

/**
 * add startup routines to the dsq for done phase
 */
void fill_done_startup(){
	
	dsq.add_routine(0, 100, R_Heartbeat);
}

/**
 * Switches to the main flight phase, ARMED
 */
void switch_to_main(){
	dsq.clear();
	fill_main_startup();
}

/**
 * Switches to done phase after mission success
 */
void switch_to_done(){
	dsq.clear();
	fill_done_startup();
}

/**
 * Switches to safe phase on command
 */
void switch_to_safe(){
	dsq.clear();
	fill_safe_startup();
}

/**
 * Routine sets modes of pin(s):
 * 		HEARTBEAT_LED
 */
void init_misc_pins(){
	pinMode(HEARTBEAT_LED, OUTPUT);
}

/**
 * Checks health of power system, adjusts power source accordantly.
 */
void power_system_check(){
	//TODO
}

void setup() {
	// TODO
	// Initialize hardware modules
	init_misc_pins();
	//power_system_check();
	init_LoRa();
	init_BME280();
	init_LSM9DS1();
	//init_CCS811();
	init_LIS331();
	init_StratoLogger();
	init_SD();
	init_deploy_pins(); // Initialize pins which deployment switches are attached.

	// Add Default routine to the dsq
	dsq.set_default(1, R_Default);

	fill_main_startup();
}

/**
 * Main Arduino loop function, calls execution system call
 */
void loop() {
	// Execute routine placed into the DSQ
	dsq.execute();
}
