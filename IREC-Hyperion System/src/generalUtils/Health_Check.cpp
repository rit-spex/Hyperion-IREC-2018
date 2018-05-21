/*
 * File: Health_Check_Hyperion.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Used to preform health check on the payload in various stages.
 */

#include "../Pins.h"
#include "Health_Check_Hyperion.h"
#include "../IRECHYPERION.h"
#include "../Transmit_Hyperion.h"
#include <IREC_Hyperion_Protocol.h>

#define MAX_MSG_SIZE 300
#define SWITCH_DEBOUNCE 500
#define DEBOUNCE_THRES 400

/**
 * Given a switch pin, function will check if pin is open with
 * software debuff.
 * Arguments
 *      switch_pin: the pin to be tested.
 * Returns
 *      1 if switch is closed
 *      0 if switch is closed
 */
unsigned int check_switch_open(int switch_pin){
    
    int cnt = 0;

    for(size_t i = 0; i < SWITCH_DEBOUNCE; i++){
        if(digitalReadFast(switch_pin) == HIGH) cnt += 1;
    }

    if(cnt > DEBOUNCE_THRES) return 1; // Switch reads open most of the time

    return 0;
}

/**
 * Function used to check for open deployment pins
 * Returns:
 *      The number of open pins
 */
unsigned int deployment_pins_open(){
    
    int tally = 0;

    tally += check_switch_open(SWITCH_01);
    tally += check_switch_open(SWITCH_02);
    tally += check_switch_open(SWITCH_03);
    tally += check_switch_open(SWITCH_04);

    return tally;
}

/**
 * Generates a health report in relation deployment switches on arm attempt.
 * Argument
 *      pin_cnt: the number of switches open.
 */
void health_report_deploy(unsigned int pin_cnt){

    switch(pin_cnt){
        case 0:
            send_health_report("ALL SWITCHES CLOSED - PAYLOAD !ARMED!\0");
            break;
        case 1:
            send_health_report("ONE SWITCH OPEN, THREE CLOSED - PAYLOAD !ARMED!\0");
            break;
        case 2:
            send_health_report("TWO SWITCHES OPEN. TWO CLOSED - PAYLOAD !DISARMED!\0");
            break;
        case 3:
            send_health_report("THREE SWITCHES OPEN. ONE CLOSED - PAYLOAD !DISARMED!\0");
            break;
        case 4:
            send_health_report("ALL SWITCHES OPEN - PAYLOAD !DISARMED!\0");
            break;
    }
}

/**
 * Function takes in mesage to be sent to ground station,
 * uses the hyperion protocol to pack message.
 * Argument:
 *      msg: The message to be sent, null terminated string.
 */
void send_health_report(char msg[]){
    
    // Allocate space for flags
	char flags[4] = {0, 0, 0, 0};
	uint16_t time = millis()/1000;

	uint8_t buff[MAX_MSG_SIZE+HEADER_SIZE] = {0};

    IRECHYPERP::createInfoFrame(buff, flags, time, msg);

    rf95.waitPacketSent();
    transmit_data(buff, HEADER_SIZE + strlen(msg) +1);
}