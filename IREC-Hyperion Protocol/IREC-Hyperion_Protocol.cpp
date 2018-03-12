/**
 * File: IREC-Hyperion_Protocol.cpp
 * Organization: RIT Space Exploration
 * Description:
 *      Protocol to be used on the Hyperion payload.
 */

#include <stdint.h>
#include "IREC-Hyperion_Protocol.h"

#define BYTE_MASK 255
#define HEADER_SIZE 5
#define LSM9DS1_FRAME_SIZE 36
#define BME280_FRAME_SIZE 16
#define CCS811_FRAME_SIZE 4

uint8_t *IRECHYPERP::createHeader(DataFrameType type, const char *flags, uint32_t time) {
    uint8_t buff[HEADER_SIZE];
    int cnt = 0;

    // Load type into first four bits
    buff[cnt] <<= 4;
    buff[cnt] |= type;

    // Load flags in last four bits of slot one
    for (int i = 0; i < 4; ++i) {
        buff[cnt] <<= 1;

        if(flags[i]){
            buff[cnt] |= 1;
        }
    }

    cnt += 1;

    // Load the time into the buffer
    for (int j = 0; j < sizeof(uint32_t); ++j) {
        cnt += j;
        buff[cnt] <<= sizeof(uint8_t)*8; // Clear the buffer

        buff[cnt] = (uint8_t)((time >> (sizeof(unsigned long) - j - 1)*8) & BYTE_MASK);
    }
    
    return buff;
}

uint8_t *IRECHYPERP::createLSM9DS1Frame(char *flags, uint32_t time,
                                     int32_t ax, int32_t ay, int32_t az,
                                     int32_t gx, int32_t gy, int32_t gz,
                                     int32_t mx, int32_t my, int32_t mz) {
    
    uint8_t *buff_header = createHeader(LSM9DS1, flags, time);
    uint8_t buff[LSM9DS1_FRAME_SIZE+HEADER_SIZE];

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
    }

    cnt += 1;

    int32_t val_arry[9] = {ax, ay, az, gx, gy, gz, mx, my, mz};

    for (int j = 0; j < 9; ++j) {
        for (int k = 0; k < sizeof(int32_t); ++k) {
            buff[cnt+k] <<= sizeof(uint8_t)*8; // Clear the buffer

            buff[cnt+k] = (uint8_t)((val_arry[j] >> (sizeof(int32_t) - k - 1)*8) & BYTE_MASK);
        }
        cnt += sizeof(int32_t);
    }

    return buff;
}

uint8_t *IRECHYPERP::createBME280Frame(char *flags, uint32_t time, int32_t temp, int32_t humidity, int32_t pressure,
                                    int32_t altitude) {

    uint8_t *buff_header = createHeader(BME280, flags, time);
    uint8_t buff[BME280_FRAME_SIZE+HEADER_SIZE];

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
    }

    cnt += 1;

    int32_t val_arry[4] = {temp, pressure, humidity, altitude}; // order matters

    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < sizeof(int32_t); ++k) {
            buff[cnt+k] <<= sizeof(uint8_t)*8; // Clear the buffer

            buff[cnt+k] = (uint8_t)((val_arry[j] >> (sizeof(int32_t) - k - 1)*8) & BYTE_MASK);
        }
        cnt += sizeof(int32_t);
    }

    return buff;
}

uint8_t *IRECHYPERP::createCCS811Frame(char *flags, uint32_t time, int16_t co2, int16_t TVOC) {

    uint8_t *buff_header = createHeader(CCS811, flags, time);
    uint8_t buff[CCS811_FRAME_SIZE+HEADER_SIZE];

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
    }

    cnt += 1;

    int16_t val_arry[2] = {co2, TVOC};

    for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < sizeof(int16_t); ++k) {
            buff[cnt+k] <<= sizeof(uint8_t)*8; // Clear the buffer

            buff[cnt+k] = (uint8_t)((val_arry[j] >> (sizeof(int16_t) - k - 1)*8) & BYTE_MASK);
        }
        cnt += sizeof(int16_t);
    }
}