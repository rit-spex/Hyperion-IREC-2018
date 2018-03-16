/*
 * IRECHYPERION_Routines
 * File: IRECHYPERION_Routines.ino
 * Organization: RIT Space Exploration
 * Desc:
 *    Routines to be included into the DSQ
 */


void R_Default(){
	// TODO
}

/**
 * Transmit route for the BME280 data frame
 */
void R_trans_BME280(){
    // Allocate space for flags
    char flags[4] = {0, 0, 0, 0};
    uint32_t time = 800; // Change

    uint8_t buff[BME280_FRAME_SIZE+HEADER_SIZE] = {0};
	//TODO
    IRECHYPERP::createBME280Frame(buff, flags, time, (int32_t)get_Temp(), (int32_t)get_Pressure(), (int32_t)get_Humidity(), (int32_t)get_BME280_Alt());

    // Transmit data via LoRa
    transmit_data(buff, BME280_FRAME_SIZE+HEADER_SIZE);

    // REMOVE
    Serial.print("Transmitted data ");Serial.println(get_Humidity());

    // Add routine back into the DSQ
    dsq.add_routine(0, 3, R_trans_BME280); 
}

