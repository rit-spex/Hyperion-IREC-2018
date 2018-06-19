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
#include "generalUtils/Health_Check_Hyperion.h"
#include "Routines_Hyperion.h"
#include "Transmit_Hyperion.h"
#include "Data_Buffer_Hyperion.h"

#define DSQ_MAIN_CAP 100
#define BATT_SWITCH 9.9

// Current DSQ in use
DSQ dsq(DSQ_MAIN_CAP);// Dynamic Scheduling Queue (DSQ)

/**
 * add startup routines to the dsq for main phase
 */
void fill_main_startup(){
	//TODO
	// Add startup routines into the dsq
	//dsq.add_routine(0, 3, R_trans_LSM9DS1);
	dsq.add_routine(0, 20, R_trans_BME280);
	//dsq.add_routine(0, 30, R_trans_CCS811);
	//dsq.add_routine(0, 10, R_trans_LIS331);
	dsq.add_routine(0, 10, R_trans_Altitude);
	dsq.add_routine(0, 10, R_trans_Orientation);
	dsq.add_routine(0, 3, R_seq_LSM9DS1_data);
	dsq.add_routine(0, 20, R_seq_BME280_data);
	//dsq.add_routine(0, 30, R_seq_CCS811_data);
	//dsq.add_routine(0, 3, R_seq_LIS331_data);
	dsq.add_routine(0, 100, R_calc_RateOfClimb);
	dsq.add_routine(0, 10, R_Altitude_data);
	dsq.add_routine(0, 10, R_seq_Altitude_data);
	dsq.add_routine(0, 1, R_check_deployment);
	dsq.add_routine(0, 3, R_gath_LSM9DS1_data);
	dsq.add_routine(0, 20, R_gath_BME280_data);
	//dsq.add_routine(0, 3, R_gath_LIS331_data);
	dsq.add_routine(0, 50, R_recv_Disarm);
	dsq.add_routine(0, 100, R_Heartbeat);
	dsq.add_routine(0, 50, R_Strobe);
}

/**
 * add startup routines to the dsq for suspended phase
 */
void fill_safe_startup(){
	
	dsq.add_routine(0, 100, R_calc_RateOfClimb);
	dsq.add_routine(0, 10, R_Altitude_data);
	dsq.add_routine(0, 3, R_gath_LSM9DS1_data);
	dsq.add_routine(0, 20, R_gath_BME280_data);
	//dsq.add_routine(0, 3, R_gath_LIS331_data);
	dsq.add_routine(0, 3, R_trans_LSM9DS1);
	dsq.add_routine(0, 20, R_trans_BME280);
	//dsq.add_routine(0, 30, R_trans_CCS811);
	//dsq.add_routine(0, 10, R_trans_LIS331);
	dsq.add_routine(0, 10, R_trans_Altitude);
	dsq.add_routine(0, 10, R_trans_Orientation);
	dsq.add_routine(0, 50, R_recv_Arm);
	dsq.add_routine(0, 100, R_Heartbeat);
	dsq.add_routine(0, 200, R_Health_report);
}

/**
 * add startup routines to the dsq for done phase
 */
void fill_done_startup(){
	
	dsq.add_routine(0, 100, R_Heartbeat);
	dsq.add_routine(0, 50, R_Strobe);
	dsq.add_routine(0, 1, R_Delay);
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

	digitalWriteFast(BUZZER_DISABLE, LOW); // Disable Buzzer
	digitalWriteFast(STROBE_DISABLE, LOW); // Disable Strobe

	dsq.clear();
	fill_safe_startup();
}

/**
 * Routine sets modes of pin(s):
 * 		LED_BLUE, LED_GREEN, LED_RED
 */
void init_misc_pins(){
	// Init Heartbeat led
	pinMode(LED_BLUE, OUTPUT);
	digitalWriteFast(LED_BLUE, LOW);

	// Init SD IO led
	pinMode(LED_GREEN, OUTPUT);
	digitalWriteFast(LED_GREEN, LOW);

	pinMode(LED_RED, OUTPUT);
	digitalWriteFast(LED_RED, LOW);

	// Init for strobe led
	pinMode(STROBE_DISABLE, OUTPUT);
	digitalWriteFast(STROBE_DISABLE, LOW);

	// Init for buzzer
	pinMode(BUZZER_DISABLE, OUTPUT);
	digitalWriteFast(BUZZER_DISABLE, LOW);

	pinMode(MAIN_BATT_EN, OUTPUT);
	digitalWriteFast(MAIN_BATT_EN, LOW);
}

/**
 * Checks health of power system, adjusts power source accordantly.
 */
void power_system_check(){
	
	float mainBattVolt = analogRead(MAIN_BATT_ANLG);
	mainBattVolt *= (3.3 / 1023.0);
	mainBattVolt /= 0.23866;

	if(mainBattVolt > BATT_SWITCH){ // Main battery ok
		digitalWriteFast(MAIN_BATT_EN, HIGH);
	} else {
		digitalWriteFast(LED_RED, HIGH);
		digitalWriteFast(MAIN_BATT_EN, LOW);
		send_health_report("MAIN BATTERY IS BELOW VOLTAGE THRESHOLD!\0");
	}
}

void setup() {
	delay(4000);
	analogReadAveraging(16); // Smooths out analog readings

	// TODO
	// Initialize hardware modules
	init_misc_pins();
	init_para_pins();
	init_LoRa();
	init_BME280();
	init_LSM9DS1();
	//init_CCS811();
	init_LIS331();
	init_StratoLogger();
	init_SD();
	init_deploy_pins(); // Initialize pins which deployment switches are attached.
	power_system_check();

	// Add Default routine to the dsq
	dsq.set_default(1, R_Default);

	fill_safe_startup();
}

/**
 * Main Arduino loop function, calls execution system call
 */
void loop() {
	// Execute routine placed into the DSQ
	dsq.execute();
}
