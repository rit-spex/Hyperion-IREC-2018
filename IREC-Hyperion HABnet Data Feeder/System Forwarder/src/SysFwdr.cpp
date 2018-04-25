/**
 * Hyperion - IREC
 * File: SysFwdr.cpp
 * Organization: RIT Space Exploration
 * Desc
 *    	Main program used to forward packets from LoRa Module to 
 *		desktop
 */

#include <SPI.h>

#define BAUD_RATE 115200

/**
 * Grabs source from transmission, unpacks data then 
 * converts packed data a comma seperated string.
 * CSV format: 
 * [type],[flag1(1,0)][flag2(1,0)][flag3(1,0)][flag4(1,0)],[time(seconds)],[data],[data]...
 * Arguments
 * 		buff, the buffer which the csv string will be placed.
 * 		source, the raw bytes from the payload.
 * Return
 * 		The number of bytes within the buff
 */
int convert_to_csv(char* buff, const char* source){

}

/**
 * Init function for the lora.
 */
void init_LoRa(){

}

/**
 * Main initilize function for the SysFwdr program.
 */
void init(){
	init_LoRa();
}

int main(){
	// Place startup code here
	Serial.begin(BAUD_RATE);

	init();

	while(true){
		// Place loop code here
	}
}