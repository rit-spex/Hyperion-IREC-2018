/**
 * File: IREC-Hyperion_Protocol.cpp
 * Organization: RIT Space Exploration
 * Description:
 *      Protocol to be used on the Hyperion payload.
 */

#include "IREC_Hyperion_Protocol.h"

void IRECHYPERP::createHeader(uint8_t buff[], DataFrameType type, const char *flags, uint32_t time) {
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
        buff[cnt+j] <<= sizeof(uint8_t)*8; // Clear the buffer

        buff[cnt+j] = (uint8_t)((time >> (sizeof(unsigned long) - j - 1)*8) & BYTE_MASK);
    }
}

void IRECHYPERP::createLSM9DS1Frame(uint8_t buff[], char *flags, uint32_t time,
                                     int32_t ax, int32_t ay, int32_t az,
                                     int32_t gx, int32_t gy, int32_t gz,
                                     int32_t mx, int32_t my, int32_t mz) {

    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, LSM9DS1t, flags, time);

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

            buff[cnt+k] = (uint8_t)((val_arry[j] >> (sizeof(int32_t) - k - 1)*8) & BYTE_MASK);
        }
        cnt += sizeof(int32_t);
    }
}

void IRECHYPERP::createBME280Frame(uint8_t buff[], char *flags, uint32_t time, int32_t temp, int32_t pressure, int32_t humidity,
                                    int32_t altitude) {

    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, BME280t, flags, time);

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

}

void IRECHYPERP::createCCS811Frame(uint8_t buff[], char *flags, uint32_t time, int16_t co2, int16_t TVOC) {

    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, CCS811t, flags, time);

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

unpk_header IRECHYPERP::unpack_header(const uint8_t buff[]) {
    unpk_header header={0};

    int cnt = 0;
    header.data_type = (buff[cnt]>>4);

    header.flags = (uint8_t) (buff[cnt] & NIBBLE_MASK);

    cnt += 1;

    for (int i = cnt; i < HEADER_SIZE; ++i) {
        header.time <<= 8;
        header.time |= buff[i];
    }

    return header;
}

LSM9DS1_Packet IRECHYPERP::unpack_LSM9DS1(const uint8_t buff[]) {
    LSM9DS1_Packet packet={0};

    packet.header = unpack_header(buff);

    uint8_t sliced_arry[LSM9DS1_FRAME_SIZE]={};

    int cnt = 0;
    for (int i = HEADER_SIZE; i < HEADER_SIZE+LSM9DS1_FRAME_SIZE; ++i) {
        sliced_arry[cnt] = buff[i];
        cnt += 1;
    }

    packet.data = unpack_LSM9DS1_Data(sliced_arry);

    return packet;
}

LSM9DS1_Data IRECHYPERP::unpack_LSM9DS1_Data(const uint8_t buff[]) {
    LSM9DS1_Data data={0};

    int32_t *val_buff[9] = {&data.ax, &data.ay, &data.az, &data.gx, &data.gy, &data.gz, &data.mx, &data.my, &data.mz};

    int cnt = 0;

    for (int i = 0; i < 9; ++i){
        for (int j = 0; j < sizeof(int32_t); ++j) {
            *val_buff[i] = *val_buff[i] << sizeof(uint8_t)*8;
            *val_buff[i] |= buff[j+cnt];
        }
        cnt += sizeof(int32_t);
    }

    return data;
}


BME280_Packet IRECHYPERP::unpack_BME280(const uint8_t *buff) {
    BME280_Packet packet={0};

    packet.header = unpack_header(buff);

    uint8_t sliced_arry[BME280_FRAME_SIZE]={};

    int cnt = 0;
    for (int i = HEADER_SIZE; i < HEADER_SIZE+BME280_FRAME_SIZE; ++i) {
        sliced_arry[cnt] = buff[i];
        cnt += 1;
    }

    packet.data = unpack_BME280_Data(sliced_arry);

    return packet;
}

BME280_Data IRECHYPERP::unpack_BME280_Data(const uint8_t *buff) {
    BME280_Data data={0};

    int32_t *val_buff[9] = {&data.temperature, &data.pressure, &data.humidity, &data.altitude};

    int cnt = 0;

    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < sizeof(int32_t); ++j) {
            *val_buff[i] = *val_buff[i] << sizeof(uint8_t)*8;
            *val_buff[i] |= buff[j+cnt];
        }
        cnt += sizeof(int32_t);
    }

    return data;
}

