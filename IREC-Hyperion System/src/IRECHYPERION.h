/*
 * Hyperion - IREC
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

#define DEFAULT_STR_LEN 200

#define DONE_PHASE_TRANS_TIME 1200000 // 20 minutes in milli seconds

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
