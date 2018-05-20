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
 * Function takes in mesage to be sent to ground station,
 * uses the hyperion protocol to pack message.
 * Argument:
 *      msg: The message to be sent
 */
void send_health_report(const char msg[]){

}