/*
 * File: Mission_Utils.cpp
 * Organization: RIT Space Exploration
 * Description:
 *     Utilities functions mission events
 */

#ifndef Mission_Utils_Hyperion_h
#define Mission_Utils_Hyperion_h

#include "IRECHYPERION.h"

int       init_deploy_pins(); // Initialize the deployment switch pins
uint32_t  deployed_delta(); // Delta time from deployment
void      set_deployment(); // Setter function for deployment;
void      rate_of_climb(); // Rate of climb
float     get_rate_of_climb(); // Getter for rate of climb
void      set_parachute_deploy();
void      set_imp_damper_deploy();
bool      get_parachute_deploy();
bool      get_imp_damper_deploy();
bool      correct_orientation_para(); // Check if orientation is okay before deploying parachute
bool      detect_launch(); // Check if launch has occurred
int       init_para_pins();

#endif
