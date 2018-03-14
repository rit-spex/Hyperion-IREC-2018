/*
 * IRECHYPERION_Routines
 * File: IRECHYPERION_Routines.ino
 * Organization: RIT Space Exploration
 * Desc:
 *    Routines to be included into the DSQ
 */

void R_trans_BME280(){
    char flags[4] = {0, 0, 0, 0};
    uint32_t time = 800;

    uint8_t buff[BME280_FRAME_SIZE+HEADER_SIZE] = {0};
    IRECHYPERP::createBME280Frame(buff, flags, time, (int32_t)get_Temp(), (int32_t)get_Pressure(), (int32_t)get_Humidity, (int32_t)get_BME280_Alt());
    
    //transmit_data(buff, BME280_FRAME_SIZE+HEADER_SIZE);

    Serial.print("[BME280] - Temp ");
    Serial.println((int32_t)get_Temp());

    dsq.add_routine(0, 3, R_trans_BME280);
}

