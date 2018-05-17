/**
 * File: IREC-Hyperion_Protocol.cpp
 * Organization: RIT Space Exploration
 * Description:
 *      Protocol to be used on the Hyperion payload.
 */

#include "IREC_Hyperion_Protocol.h"

void IRECHYPERP::createHeader(uint8_t buff[], DataFrameType type, const char *flags, uint16_t time) {
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
    for (int j = 0; j < sizeof(uint16_t); ++j) {
        buff[cnt+j] <<= sizeof(uint8_t)*8; // Clear the buffer

        buff[cnt+j] = (uint8_t)((time >> (sizeof(uint16_t) - j - 1)*8) & BYTE_MASK);
    }
}

void IRECHYPERP::createLSM9DS1Frame(uint8_t buff[], char *flags, uint16_t time,
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

void IRECHYPERP::createBME280Frame(uint8_t buff[], char *flags, uint16_t time, int32_t temp, int32_t pressure, int32_t humidity,
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

void IRECHYPERP::createCCS811Frame(uint8_t buff[], char *flags, uint16_t time, int16_t co2, int16_t TVOC) {

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

void IRECHYPERP::createLIS311Frame(uint8_t buff[], char *flags, uint16_t time, int32_t ax, int32_t ay, int32_t az) {

    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, LIS331t, flags, time);

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
    }

    cnt += 1;

    int32_t val_arry[3] = {ax, ay, az};

    for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < sizeof(int32_t); ++k) {
            buff[cnt+k] <<= sizeof(uint8_t)*8; // Clear the buffer

            buff[cnt+k] = (uint8_t)((val_arry[j] >> (sizeof(int32_t) - k - 1)*8) & BYTE_MASK);
        }
        cnt += sizeof(int32_t);
    }
}

void IRECHYPERP::createPFSLFrame(uint8_t *buff, char *flags, uint16_t time, int32_t altitude) {

    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, PFSLt, flags, time);

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
    }

    cnt += 1;

    for (int k = 0; k < sizeof(int32_t); ++k) {
        buff[cnt+k] <<= sizeof(uint8_t)*8; // Clear the buffer

        buff[cnt+k] = (uint8_t)((altitude >> (sizeof(int32_t) - k - 1)*8) & BYTE_MASK);
    }
}


void IRECHYPERP::createOrenFrame(uint8_t *buff, char *flags, uint16_t time, int32_t pitch, int32_t roll, int32_t yaw) {

    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, Orent, flags, time);

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
    }

    cnt += 1;

    int32_t val_arry[3] = {pitch, roll, yaw};

    buff[cnt] <<= sizeof(uint8_t)*8; // clear the buffer

    // Pack negative holders
    for (int j = 0; j < 3; ++j) {
        buff[cnt] <<= 1;
        if(val_arry[j] < 0){
            buff[cnt] |= 1;
            val_arry[j] *= -1;
        }
    }

    cnt += 1;

    for (int k = 0; k < 3; ++k) {
        buff[cnt+k] <<= sizeof(uint8_t)*8;
        buff[cnt+k] |= val_arry[k];
    }
}

void IRECHYPERP::createCMMNDFrame(uint8_t *buff, char *flags, uint16_t time) {
    uint8_t buff_header[HEADER_SIZE] = {0};
    createHeader(buff_header, CMMNDt, flags, time);

    int cnt = 0;

    // Pack header into buffer
    for (int i = 0; i < HEADER_SIZE; ++i) {
        cnt = i;
        buff[cnt] = buff_header[cnt];
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

    int32_t *val_buff[4] = {&data.temperature, &data.pressure, &data.humidity, &data.altitude};

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

CCS811_Packet IRECHYPERP::unpack_CCS811(const uint8_t *buff) {
    CCS811_Packet packet = {0};
    
    packet.header = unpack_header(buff);

    uint8_t sliced_arry[CCS811_FRAME_SIZE] = {};

    int cnt = 0;
    for (int i = HEADER_SIZE; i < HEADER_SIZE+CCS811_FRAME_SIZE; i++) {
        sliced_arry[cnt] = buff[i];
        cnt += 1;
    }

    packet.data = unpack_CCS811_Data(sliced_arry);
    return packet;
}

CCS811_Data IRECHYPERP::unpack_CCS811_Data(const uint8_t *buff) {
    CCS811_Data data={0};

    int16_t *val_buff[2] = {&data.co2, &data.TVOC};

    int cnt = 0;

    for (int i = 0; i < 2; ++i){
        for (int j = 0; j < sizeof(int16_t); ++j) {
            *val_buff[i] = *val_buff[i] << sizeof(uint8_t)*8;
            *val_buff[i] |= buff[j+cnt];
        }
        cnt += sizeof(int16_t);
    }

    return data;
}

LIS311_Packet IRECHYPERP::unpack_LIS311(const uint8_t *buff) {
    LIS311_Packet packet = {0};

    packet.header = unpack_header(buff);

    uint8_t sliced_arry[LIS331_FRAME_SIZE] = {};

    int cnt = 0;
    for (int i = HEADER_SIZE; i < HEADER_SIZE+LIS331_FRAME_SIZE; i++) {
        sliced_arry[cnt] = buff[i];
        cnt += 1;
    }

    packet.data = unpack_LIS311_Data(sliced_arry);
    return packet;
}

LIS311_Data IRECHYPERP::unpack_LIS311_Data(const uint8_t *buff) {
    LIS311_Data data={0};

    int32_t *val_buff[3] = {&data.ax, &data.ay, &data.az};

    int cnt = 0;

    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < sizeof(int32_t); ++j) {
            *val_buff[i] = *val_buff[i] << sizeof(uint8_t)*8;
            *val_buff[i] |= buff[j+cnt];
        }
        cnt += sizeof(int32_t);
    }

    return data;
}


PFSL_Packet IRECHYPERP::unpack_PFSL(const uint8_t *buff) {
    PFSL_Packet packet = {0};

    packet.header = unpack_header(buff);

    uint8_t sliced_arry[PFSL_FRAME_SIZE] = {};

    int cnt = 0;
    for (int i = HEADER_SIZE; i < HEADER_SIZE+PFSL_FRAME_SIZE; i++) {
        sliced_arry[cnt] = buff[i];
        cnt += 1;
    }

    packet.data = unpack_PFSL_Data(sliced_arry);
    return packet;
}

PFSL_Data IRECHYPERP::unpack_PFSL_Data(const uint8_t *buff) {
    PFSL_Data data={0};

    for (int j = 0; j < sizeof(int32_t); ++j) {
        data.alt = data.alt << sizeof(uint8_t)*8;
        data.alt |= buff[j];
    }

    return data;
}

OREN_Packet IRECHYPERP::unpack_Oren(const uint8_t *buff) {
    OREN_Packet packet = {0};

    packet.header = unpack_header(buff);

    uint8_t sliced_arry[OREN_FRAME_SIZE] = {};

    int cnt = 0;
    for (int i = HEADER_SIZE; i < HEADER_SIZE+OREN_FRAME_SIZE; i++) {
        sliced_arry[cnt] = buff[i];
        cnt += 1;
    }

    packet.data = unpack_Oren_Data(sliced_arry);
    return packet;
}

OREN_Data IRECHYPERP::unpack_Oren_Data(const uint8_t *buff) {

    OREN_Data data = {0};

    int16_t *val_buff[3] = {&data.pitch, &data.roll, &data.yaw};

    for (int i = 0; i < 3; ++i) {
        int mult = 1;
        if((buff[0] >> (2 - i)) & 1){
            mult = -1;
        }
        (*val_buff[i]) = (int16_t) buff[1 + i] * mult;
    }

    return data;
}

CMMND_Packet IRECHYPERP::unpack_CMMND(const uint8_t *buff) {
    CMMND_Packet packet = {0};

    packet.header = unpack_header(buff);

    return packet;
}

