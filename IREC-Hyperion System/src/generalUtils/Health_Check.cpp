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

/**
 * Function used to check for open deployment pins
 * Returns:
 *      The number of open pins
 */
unsigned int deployment_pins_open(){
    
    int tally = 0;

    if(digitalReadFast(DEPLOY_SWITCH_01) == HIGH) tally += 1;
    if(digitalReadFast(DEPLOY_SWITCH_02) == HIGH) tally += 1;
    if(digitalReadFast(DEPLOY_SWITCH_03) == HIGH) tally += 1;
    if(digitalReadFast(DEPLOY_SWITCH_04) == HIGH) tally += 1;

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