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
#include "sensorUtils/GPS_Hyperion.h"
#include "generalUtils/Mission_Utils_Hyperion.h"
#include "sensorUtils/StratoLogger_Hyperion.h"
#include "generalUtils/Routine_Helpers_Hyperion.h"
#include "Transmit_Hyperion.h"
#include "generalUtils/Health_Check_Hyperion.h"
#include "Data_Buffer_Hyperion.h"
#include "Pins.h"

#define D_ERROR_SPEED -20 // -20 m/s

#define ALTITUDE_MIN_PRI 1
#define ALTITUDE_MAX_PRI 400

#define SWITCH_DEBOUNCE 5000

#define ALT_DEBOUNCE 50

#define PARA_TIMEOUT 3000
#define PARA_TIMEOUT_FIN 10000
#define PARA_BLAST_TIME 2000

#define DAMPER_DEPLOY_SPEED -15
#define DAMPER_TIMEOUT 40000

#define DDEBOUNCE_TIME 1000
#define DSEQ_CHECKS 5

#define HEARTBEAT_INT 1000
#define STROBE_INT 250

void R_Default(){
	// TODO
}

/**
 * Routine to check deployment from launch vehicle 
 * Deployment check passes on > 2 pins open or 2 pins open and 
 * payload is traveling down faster than the defined threshold.
 * 
 * This routine will take DDEBOUNCE_TIME to complete. In this time 
 * deployment switches will be checked DSEQ_CHECKS. If the routine passes
 * DSEQ_CHECKS the R_mission_constraints is added into the DSQ 
 * and this routine is removed(Not added).
 */
void R_check_deployment(){
	static unsigned int seq_cnt = 0;
	static uint32_t seq_time = 0;

	if(!seq_cnt || (millis() - seq_time >= DDEBOUNCE_TIME/DSEQ_CHECKS)){
		unsigned int open_cnt = 0;

		// Switch debounce included
    	open_cnt += check_switch_open(SWITCH_01);
    	open_cnt += check_switch_open(SWITCH_02);
    	open_cnt += check_switch_open(SWITCH_03);
    	open_cnt += check_switch_open(SWITCH_04);

		if(
			open_cnt > 2 || 
			(open_cnt == 2 && get_rate_of_climb() < D_ERROR_SPEED)
		  ){
			seq_time = millis();
			++seq_cnt;
		} else {
			// Reset sequence if switches fail condition
			seq_cnt = 0;
			seq_time = 0;
		}
	}

	if(seq_cnt >= DSEQ_CHECKS){
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
		deployed_time_para = millis();
	}

	if(millis() - deployed_time_para >= PARA_BLAST_TIME){
		// Return the fire pins to low.
		digitalWriteFast(EMATCH_1_FIRE, LOW);

		if(check_para_pin()){
			delay(5000);
			digitalWriteFast(EMATCH_1_FIRE, HIGH);
			delay(2000);
			digitalWriteFast(EMATCH_1_FIRE, LOW);
		}

		digitalWriteFast(EMATCH_1_ARM, LOW);
		return;
	}

	dsq.add_routine(0, 0, R_deploy_parachute);
}

/**
 * Dummy function to simulate when impact dampers would deploy.
 */
void R_deploy_dampers(){

	set_imp_damper_deploy();
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
 * Toggles heartbeat led using predefined interval
 */
void R_Heartbeat(){
	static bool toggle = false;
	static uint32_t time_track = 0;

	if(millis() - time_track > HEARTBEAT_INT){
		if(toggle){
			toggle = false;
			digitalWriteFast(LED_BLUE, LOW);
		} else {
			toggle = true;
			digitalWriteFast(LED_BLUE, HIGH);
		}
		time_track = millis();
	}

	dsq.add_routine(0, 10, R_Heartbeat);
}

/**
 * Toggles strobe LED for predefined interval.
 */
void R_Strobe(){
	static bool toggle = false;
	static uint32_t time_track = 0;

	if(millis() - time_track > HEARTBEAT_INT){
		if(toggle){
			toggle = false;
			digitalWriteFast(STROBE_DISABLE, LOW);
			digitalWriteFast(BUZZER_DISABLE, LOW); // Enable Buzzer
		} else {
			toggle = true;
			digitalWriteFast(STROBE_DISABLE, HIGH);
			digitalWriteFast(BUZZER_DISABLE, HIGH); // Enable Buzzer
		}
		time_track = millis();
	}

	dsq.add_routine(0, 10, R_Strobe);
}

/**
 * Routine used to disarm payload remotely using the RFM_9X LoRa
 * Tx/Rx modules. This routine first sends out current phase status then 
 * listens for any commands to disarm the payload. If no command is given in 1000 milliseconds
 * the routine cycles and tries again. In the event of a launch, this routine will be removed from execution.
 */
void R_recv_Disarm(){

	// Remove routine from phase if launch has been detected
	if(detect_launch()) return;

	char flags[4] = {0, 0, 1, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[HEADER_SIZE] = {0};

	IRECHYPERP::createCMMNDFrame(buff, flags, time);

	rf95.waitPacketSent();
	transmit_data(buff, HEADER_SIZE);
	rf95.waitPacketSent();

	uint8_t msg_buff[HEADER_SIZE] = {0};
	uint8_t len = sizeof(msg_buff);

	// Receive command from ground station.
	if (rf95.waitAvailableTimeout(500)){
		if(rf95.recv(msg_buff, &len)){
			if(IRECHYPERP::typeofData(msg_buff) == CMMNDt){
				CMMND_Packet packet = IRECHYPERP::unpack_CMMND(msg_buff);

				if((packet.header.flags >> 2) & 1){
					switch_to_safe(); // Disarm the payload
					return;
				}
			}
		}
	}

	dsq.add_routine(0, 50, R_recv_Disarm);
}

/**
 * Routine used to arm the payload, first a status packet is sent out to the ground 
 * station. Then the payload listens for a command update packet, if the packet is 
 * an am command, the payload switches over to armed mode (main mode).
 */
void R_recv_Arm(){

	char flags[4] = {0, 0, 0, 1};
	uint16_t time = millis()/1000;

	uint8_t buff[HEADER_SIZE] = {0};

	IRECHYPERP::createCMMNDFrame(buff, flags, time);
	rf95.waitPacketSent();
	transmit_data(buff, HEADER_SIZE);
	rf95.waitPacketSent();

	uint8_t msg_buff[HEADER_SIZE] = {0};
	uint8_t len = sizeof(msg_buff);

	// Receive command from ground station.
	if (rf95.waitAvailableTimeout(500) &&
		rf95.recv(msg_buff, &len) && 
		IRECHYPERP::typeofData(msg_buff) == CMMNDt){

			CMMND_Packet packet = IRECHYPERP::unpack_CMMND(msg_buff);

			// If the packet is a Arm command
			if((packet.header.flags >> 3) & 1){

				if(arm_check_send()){
					send_report("ALL CRITCAL SUBSYSTEMS NOMINAL - ARMING PAYLOAD\0");
					switch_to_main(); // Arm the payload
					return;
				}
			}
		}

	dsq.add_routine(0, 50, R_recv_Arm);
}

/**
 * Delay routine used to block cpu when not needed.
 */
void R_Delay(){

	delay(1);

	dsq.add_routine(0, 1, R_Delay);
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
 * Gathers gps data
 */
void R_gath_GPS_data(){

	gather_gps_data();
	dsq.add_routine(0, 10, R_gath_GPS_data);
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
	static bool first_send = false;

	int bytes_read = read_HWSERIAL_Strato();

	if(bytes_read == 0){
		not_read_cnt += 1;
	} else {
		not_read_cnt = 0; // Reset
	}

	// Use alt from BME280 if stratologger is offline
	if(not_read_cnt >= ALT_DEBOUNCE) {
		if(!first_send) send_report("USING BME280 ALTITUDE DATA!\0");
		first_send = true;
		update_alt_BME280();
		dsq.add_routine(0, 50, R_Altitude_data);
		return;
	}

	first_send = false;
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
 * Reporting for critical sub-systems to the ground station.
 */
void R_Health_report(){
	if(arm_check_send()){
		send_report("ALL SUB-SYSTEMS NOMINAL - GO FOR ARM\0");
	} else {
		send_report("CRITICAL SUBSYSTEM FAILURE\0");
	}

	dsq.add_routine(0, 200, R_Health_report);
}

void R_trans_GPS(){

	char buff[200] = {'\0'};
	char temp[20] = {'\0'};

	itoa(gps.time.hour(), temp, 10);
	strcat(buff, temp);
	strcat(":\0", buff);
	itoa(gps.time.minute(), temp, 10);
	strcat(buff, temp);
	strcat(":\0", buff);
	itoa(gps.time.second(), temp, 10);
	strcat(buff, temp);
	strcat(",\0", buff);
	dtostrf(gps.location.lat(), 0, 10, temp);
	strcat(buff, temp);
	strcat(",\0", buff);
	dtostrf(gps.location.lng(), 0, 10, temp);
	strcat(buff, temp);
	strcat(",\0", buff);
	dtostrf(gps.altitude.meters(), 0, 10, temp);
	strcat(buff, temp);
	
	send_report(buff);

	dsq.add_routine(0, get_bandwidth_scaler() * 15, R_trans_GPS);
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

	dsq.add_routine(0, get_bandwidth_scaler()+10, R_trans_LSM9DS1);
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
	dsq.add_routine(0, get_bandwidth_scaler()+6, R_trans_BME280);
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
	dsq.add_routine(0, get_bandwidth_scaler()+7, R_trans_CCS811);
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
	dsq.add_routine(0, get_bandwidth_scaler()+4, R_trans_LIS331);
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
	dsq.add_routine(0, get_bandwidth_scaler()+5, R_trans_Altitude);
}

/**
 * Transmit routine for orientation.
 */
void R_trans_Orientation(){
	// Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[OREN_FRAME_SIZE+HEADER_SIZE] = {0};	

	IRECHYPERP::createOrenFrame(buff, flags, time, 
	convert_float_int32(calc_Pitch_Deg()), convert_float_int32(calc_Roll_Deg()), convert_float_int32(calc_Heading_Deg()));

	transmit_data(buff, OREN_FRAME_SIZE+HEADER_SIZE);

	dsq.add_routine(0, get_bandwidth_scaler(), R_trans_Orientation);
}
