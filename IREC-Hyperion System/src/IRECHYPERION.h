/*
 * IREC - Hyperion
 * Organization: RIT Space Exploration
 * Desc
 *    Main header file for the Hyperion system
 */

#ifndef IRECHYPERION
#define IRECHYPERION

// Included libaries
#include <string.h>
#include <Wire.h>
#include <SPI.h>
#include <IREC_Hyperion_Protocol.h>
#include <DSQ.h>

////////////////////
// Define Macros //
//////////////////

// File to be created and written
#define FILE_NAME "TEST"

#define LED 13 // LED pin

#define DEFAULT_STR_LEN 200

#define DONE_PHASE_TRANS_TIME 1200000 // 20 minutes in milli seconds

#define DEPLOY_SWITCH_01 25 
#define DEPLOY_SWITCH_02 26 
#define DEPLOY_SWITCH_03 27 
#define DEPLOY_SWITCH_04 28 

#define HEARTBEAT_LED 13 // Change

//////////////////////
// Global Varables //
////////////////////

extern DSQ dsq; // Dynamic Scheduling Queue(DSQ)

//////////////
/// Enums ///
////////////

// Used to define "no data" buffer writes
enum NoData_Type{
  DEPLOYMENT,
  PARACHUTE_DEPLOY,
  DAMPER_DEPLOY
};

//////////////////////////
// Function prototypes //
////////////////////////

// *********************************
// main

void      switch_to_main();
void      switch_to_done();
void      switch_to_safe();

#endif
