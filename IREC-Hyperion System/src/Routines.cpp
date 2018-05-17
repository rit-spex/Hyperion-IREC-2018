/*
 * Routines
 * File: Routines.cpp
 * Organization: RIT Space Exploration
 * Desc:
 *    Routines to be included into the DSQ
 */

#include "Routines_Hyperion.h"
#include "IRECHYPERION.h"
#include "sensorUtils/LSM9DS1_Hyperion.h"
#include "sensorUtils/BME280_Hyperion.h"
#include "sensorUtils/CCS811_Hyperion.h"
#include "sensorUtils/LIS331_Hyperion.h"
#include "generalUtils/Mission_Utils_Hyperion.h"
#include "sensorUtils/StratoLogger_Hyperion.h"
#include "generalUtils/Routine_Helpers_Hyperion.h"
#include "Transmit_Hyperion.h"
#include "Data_Buffer_Hyperion.h"
#include "Pins.h"

#define DEPLOYMENT_ERROR_SPEED -20 // -20 m/s

#define ALTITUDE_MIN_PRI 1
#define ALTITUDE_MAX_PRI 400

#define ACCEL_AUTO_ARM_THRES 2 // In gees
#define ROC_AUTO_ARM_THRES 30 // m/s

#define SWITCH_DEBUFF 5000

#define ALT_DEBUFF 1000

#define PARA_TIMEOUT 3000
#define PARA_TIMEOUT_FIN 10000
#define PARA_BLAST_TIME 2000

#define DAMPER_DEPLOY_SPEED -15
#define DAMPER_TIMEOUT 40000

void R_Default(){
	// TODO
}

/**
 * Routine to check deployment of the Hyperion payload
 * Cases:
 * open > 2: Deployment of the Hyperion payload
 * 0 < open <= 2: Anomaly case where 1 or 2 switches are open and 3 or 2 switches are still
 * closed.
 * Handling:
 * Case 1: Deployment procedure
 * Case 2: Further checking for deployment
 *    If rate of climb (Negitive) is above DEPLOYMENT_ERROR_SPEED
 *      Deployed
 */
void R_check_deployment(){

	int open_cnt = 0;
	static unsigned int switch_debuff = 0;

	if(digitalReadFast(DEPLOY_SWITCH_01) == HIGH) open_cnt += 1;
	if(digitalReadFast(DEPLOY_SWITCH_02) == HIGH) open_cnt += 1;
	if(digitalReadFast(DEPLOY_SWITCH_03) == HIGH) open_cnt += 1;
	if(digitalReadFast(DEPLOY_SWITCH_04) == HIGH) open_cnt += 1;

	if(open_cnt > 2){
		switch_debuff += 1;
		
	} else if (open_cnt == 2){
		// Anomaly case where 2 switches are open and 2 switches are still
		// closed.
		if (get_rate_of_climb() < DEPLOYMENT_ERROR_SPEED) switch_debuff += 1;
		else switch_debuff = 0;
	} else {
		switch_debuff = 0;
	}

	if(switch_debuff >= SWITCH_DEBUFF){
		set_deployment(); // Set time deployed
		dsq.add_routine(0, 1, R_mission_constraints);
		return;
	}

	dsq.add_routine(0, 1, R_check_deployment);
}

/**
 * Main routine to trigger mission events, routine is responsible for adding
 * routines: R_deploy_parachute and R_deploy_dampers.
 * Routine is also responsible for handling phase shiftting from armed(main)
 * phase to done state. 
 */
void R_mission_constraints(){
	// Parachute not deployed
	if(!get_parachute_deploy()){
		// Deployment triggers if greater than five seconds from deployment and in
		// correct orientation or if timeout occurs.
		if(((deployed_delta() > PARA_TIMEOUT) && correct_orientation_para())
		|| deployed_delta() > PARA_TIMEOUT_FIN){

			set_parachute_deploy();
			dsq.add_routine(0, 0, R_deploy_parachute);
		}
	}

	// Impact damper not deployed
	if(!get_imp_damper_deploy()){
		float roc = get_rate_of_climb();

		if((roc > DAMPER_DEPLOY_SPEED && roc < 0 && get_parachute_deploy())\
		|| deployed_delta() > DAMPER_TIMEOUT){

			set_imp_damper_deploy();
			dsq.add_routine(0, 0, R_deploy_dampers);
		}
	}

	if(deployed_delta() > DONE_PHASE_TRANS_TIME){
		switch_to_done();
		return;
	}

	dsq.add_routine(0, 1, R_mission_constraints);
}

/**
 * Function for parachute deployment, trigger pin is pulled
 * HIGH for 2000 milli seconds or 2 seconds to trigger two ematches
 */
void R_deploy_parachute(){

	static uint32_t deployed_time_para = 0;

	if(!deployed_time_para){
		// Fire the two ematches
		digitalWriteFast(EMATCH_1_FIRE, HIGH);
		digitalWriteFast(EMATCH_2_FIRE, HIGH);
		deployed_time_para = millis();
	}

	if(millis() - deployed_time_para >= PARA_BLAST_TIME){
		// Return the fire pins to low.
		digitalWriteFast(EMATCH_1_FIRE, LOW);
		digitalWriteFast(EMATCH_2_FIRE, LOW);		
		return;
	}

	dsq.add_routine(0, 0, R_deploy_parachute);
}

void R_deploy_dampers(){

}

/**
 * Calculate the rate of climb, then store this into a varable to be fetched when
 * needed
 */
void R_calc_RateOfClimb(){

	rate_of_climb(); // update rate of climb
	dsq.add_routine(0, 100, R_calc_RateOfClimb);
}

/**
 * Toggles heartbeat led
 */
void R_Heartbeat(){
	static bool toggle = false;

	if(toggle){
		toggle = false;
		digitalWriteFast(HEARTBEAT_LED, LOW);
	} else {
		toggle = true;
		digitalWriteFast(HEARTBEAT_LED, HIGH);
	}

	dsq.add_routine(0, 100, R_Heartbeat);
}

void R_recv_Disarm(){
	//TODO
	/*

	bool check_RateOfClimb = false;
	bool check_x = false;
	bool check_y = false; 
	bool check_z = false;

	read_LIS331();

	//TODO verify this with sensor output
	// accelation greater than the set threshold
	if(get_lis331_accel_x() > ACCEL_AUTO_ARM_THRES) check_x = true;
	if(get_lis331_accel_y() > ACCEL_AUTO_ARM_THRES) check_y = true;
	if(get_lis331_accel_z() > ACCEL_AUTO_ARM_THRES) check_z = true;

	if(get_rate_of_climb() > ROC_AUTO_ARM_THRES) check_RateOfClimb = true;

	if(check_RateOfClimb && (check_x || check_y || check_z)){
		switch_to_main();
		return;
	}
	*/
}

/**
 * Routine transmits status of payload to ground station, then listen for an arm command.
 */
void R_recv_Arm(){
	//TODO
	// Wait packet sent
	// Listen 300 ms for messages
	// If message received, check if is arm command
	//		If arm -> switch to main phase
	//		return
	// Transmit unarmed message to ground.
	// add this routine back to dsq
}

/**
 * Reads sensor LSM9DS1 output and stores it in programs memory space
 */
void R_gath_LSM9DS1_data(){
		// Read data from sensors
	read_LSM9DS1_Sensors();
	dsq.add_routine(0, 2, R_gath_LSM9DS1_data);
}

/**
 * Reads sensor BME280 output and stores it in programs memory space
 */
void R_gath_BME280_data(){

	read_BME280_Sensors();
	dsq.add_routine(0, 20, R_gath_BME280_data);
}

/**
 * Reads sensor LIS331 output and stores it in programs memory space
 */
void R_gath_LIS331_data(){

	read_LIS331();
	dsq.add_routine(0, 2, R_gath_LIS331_data);
}

/**
 * Gathers data then creates a data string to be added into the data buffer
 */
void R_seq_LSM9DS1_data(){

	// Create string
	// Pack string with data from the sensors
	char * data_str = form_LSM9DS1_str();

	if (data_str != NULL){
		// Insert data into the data buffer
		add_to_buffer(data_str);
	}

	dsq.add_routine(0, 2, R_seq_LSM9DS1_data);
}

/**
 * Gathers data from the BME280 util then creates a data string to be added
 * into the data buffer.
 */
void R_seq_BME280_data(){

	// Create string
	// Pack string with data from the sensors
	char * data_str = form_BME280_str();

	if (data_str != NULL){
		// Insert data into the data buffer
		add_to_buffer(data_str);
	}

	dsq.add_routine(0, 20, R_seq_BME280_data);
}

/**
 * Creates a data string for the CCS811 sensor, then places it into the BUFFER
 */
void R_seq_CCS811_data(){

	// Create string
	// Pack string with data from the sensors
	char * data_str = form_CCS811_str();

	if (data_str != NULL){
		// Insert data into the data buffer
		add_to_buffer(data_str);
	}

	dsq.add_routine(0, 20, R_seq_CCS811_data);
}

/**
 * Creates a data string for the LIS331 sensor, then places it into the BUFFER
 */
void R_seq_LIS331_data(){
	
	// Create string
	// Pack string with data from the sensors
	char * data_str = form_LIS331_str();

	if (data_str != NULL){
		// Insert data into the data buffer
		add_to_buffer(data_str);
	}

	dsq.add_routine(0, 2, R_seq_LIS331_data);
}

/**
 * Gather information from the Stratologger and log the data
 */
void R_Altitude_data(){
	static unsigned int not_read_cnt = 0;

	int bytes_read = read_HWSERIAL_Strato();

	if(bytes_read == 0){
		not_read_cnt += 1;
	} else {
		not_read_cnt = 0; // Reset
	}

	// Use alt from BME280 if stratologger is offline
	if(not_read_cnt >= ALT_DEBUFF) {
		update_alt_BME280();
		dsq.add_routine(0, 200, R_Altitude_data);
		return;
	}

	dsq.add_routine(0, 50, R_Altitude_data);
}

/**
 * Altitude logging routine
 */
void R_seq_Altitude_data(){
	static unsigned int dynamic_pri = 10;

	if(get_new_altitude()){

		char * data_str = form_StratoLogger_str();

		if (data_str != NULL){
			// Insert data into the data buffer
			add_to_buffer(data_str);
		}
		set_old_altitude();

		if(dynamic_pri < ALTITUDE_MAX_PRI) dynamic_pri += 1;
	} else {

		if(dynamic_pri > ALTITUDE_MIN_PRI) dynamic_pri -= 1;
	}

	dsq.add_routine(0, dynamic_pri, R_seq_Altitude_data);
}

/**
 * Transmit routine for the LSM9DS1 data frame.
 */
void R_trans_LSM9DS1(){
	// Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[LSM9DS1_FRAME_SIZE+HEADER_SIZE] = {0};

	IRECHYPERP::createLSM9DS1Frame(buff, flags, time,
	convert_float_int32(get_Accel(X_AXIS)), convert_float_int32(get_Accel(Y_AXIS)), convert_float_int32(get_Accel(Z_AXIS)),
	convert_float_int32(get_Gyro(X_AXIS)), convert_float_int32(get_Gyro(Y_AXIS)), convert_float_int32(get_Gyro(Z_AXIS)),
	convert_float_int32(get_Mag(X_AXIS)), convert_float_int32(get_Mag(Y_AXIS)), convert_float_int32(get_Mag(Z_AXIS)));

	transmit_data(buff, LSM9DS1_FRAME_SIZE+HEADER_SIZE);

	dsq.add_routine(0, get_bandwidth_scaler()*10, R_trans_LSM9DS1);
}

/**
 * Transmit routine for the BME280 data frame
 */
void R_trans_BME280(){
	// Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[BME280_FRAME_SIZE+HEADER_SIZE] = {0};

	IRECHYPERP::createBME280Frame(buff, flags, time,
	convert_float_int32(get_Temp()), convert_float_int32(get_Pressure()),
	convert_float_int32(get_Humidity()), convert_float_int32(get_BME280_Alt()));

	transmit_data(buff, BME280_FRAME_SIZE+HEADER_SIZE);

	// Add routine back into the DSQ
	dsq.add_routine(0, get_bandwidth_scaler()*6, R_trans_BME280);
}

/**
 * Transmit routine for the CCS811 data frame
 */
void R_trans_CCS811(){
	// Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[CCS811_FRAME_SIZE+HEADER_SIZE] = {0};

	IRECHYPERP::createCCS811Frame(buff, flags, time, get_TVOC(), get_CO2());

	transmit_data(buff, CCS811_FRAME_SIZE+HEADER_SIZE);
	// Add routine back into the DSQ
	dsq.add_routine(0, get_bandwidth_scaler()*7, R_trans_CCS811);
}

/**
 * Transmit routine for the LIS331
 */
void R_trans_LIS331(){
	// Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[LIS331_FRAME_SIZE+HEADER_SIZE] = {0};

	IRECHYPERP::createLIS311Frame(buff, flags, time,
	convert_float_int32(get_lis331_accel_x()), 
	convert_float_int32(get_lis331_accel_y()),
	convert_float_int32(get_lis331_accel_z()));

	transmit_data(buff, LIS331_FRAME_SIZE+HEADER_SIZE);
	
	// Add routine back into the DSQ
	dsq.add_routine(0, get_bandwidth_scaler()*4, R_trans_LIS331);
}

/**
 * Transmit routine for altitude
 */
void R_trans_Altitude(){
	// Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[PFSL_FRAME_SIZE+HEADER_SIZE] = {0};

	IRECHYPERP::createPFSLFrame(buff, flags, time, 
	convert_float_int32(get_Altitude()));

	transmit_data(buff, PFSL_FRAME_SIZE+HEADER_SIZE);
		
	// Add routine back into the DSQ
	dsq.add_routine(0, get_bandwidth_scaler()*5, R_trans_Altitude);
}
