/**
 * File: IREC-Hyperion_Protocol.h
 * Organization: RIT Space Exploration
 * Description:
 *      Protocol to be used on the Hyperion payload.
 */

#ifndef IREC_HYPERION_PROTOCOL_LIBRARY_H
#define IREC_HYPERION_PROTOCOL_LIBRARY_H

#define BYTE_MASK 255
#define NIBBLE_MASK 15

#include <stdint.h>

#define HEADER_SIZE 5
#define LSM9DS1_FRAME_SIZE 36
#define BME280_FRAME_SIZE 16
#define CCS811_FRAME_SIZE 4

/////////////////////
/// Data Structs ///
///////////////////

/**
 * Unpacked header
 */
typedef struct unpk_header{
    uint8_t data_type;
    uint8_t flags;
    uint32_t time;
} unpk_header;

/**
 * Data struct for the LSM8DS1 data frame
 */
typedef struct LSM9DS1_Data{
    int32_t ax, ay, az;
    int32_t gx, gy, gz;
    int32_t mx, my, mz;
} LSM9DS1_Data;

/**
 * Data struct for the BME280 data frame
 */
typedef struct BME280_Data{
    int32_t temperature;
    int32_t pressure;
    int32_t humdity;
    int32_t altiude;
} BME280_Data;

/**
 * Data struct for the CSS811 data frame
 */
typedef struct CCS811_Data{
    int16_t co2;
    int16_t TVOC;
} CCS811_Data;

/**
 * Packet struct containing LSM9DS1 data and header.
 */
typedef struct LSM9DS1_Packet{
    unpk_header header;
    LSM9DS1_Data data;
} LSM9DS1_Packet;

/**
 * Packet struct containing BME280 data and header.
 */
typedef struct BME280_Packet{
    unpk_header header;
    BME280_Data data;
} BME280_Packet;

/**
 * Packet struct containing CCS811 data and header.
 */
typedef struct CCS811_Packet{
    unpk_header header;
    CCS811_Data data;
} CCS811_Packet;

/**
 * Data Type Enums
 */
enum DataFrameType{
    NULLDATAt,
    LSM9DS1t,
    BME280t,
    CCS811t,
    PFSLt
};

// TODO add unpacking functionality
// TODO finish packing functions for all data groupss

/**
 * Main class for IREC-HYPERION_PROTOCOL
 */
class IRECHYPERP {
public:

    /**
     * Main packer function for the LSMDS1 data frame.
     * @param flags
     *      Header flags
     * @param time
     *      Time in seconds
     * @param ax
     *      Acceleration value on the X axis
     * @param ay
     *      Acceleration value on the Y axis
     * @param az
     *      Acceleration value on the Z axis
     * @param gx
     *      Gyroscope value on the X axis
     * @param gy
     *      Gyroscope value on the Y axis
     * @param gz
     *      Gyroscope value on the Z axis
     * @param mx
     *      Magnetic value on the X axis
     * @param my
     *      Magnetic value on the Y axis
     * @param mz
     *      Magnetic value on the Z axis
     * @return
     *      A fully complete data frame with header
     */
    static void createLSM9DS1Frame(uint8_t buff[], char *flags, uint32_t time,
                       int32_t ax, int32_t ay, int32_t az,
                       int32_t gx, int32_t gy, int32_t gz,
                       int32_t mx, int32_t my, int32_t mz);

    /**
     * Main packer function for the BME280 data frame
     * @param flags
     *      Header flags
     * @param time
     *      Time in seconds (Epoch)
     * @param temp
     *      Temperature in C
     * @param humidity
     *      Humidity in percentage
     * @param pressure
     *      Pressure
     * @param altitude
     *      Altitude in m
     * @return
     *      A fully complete data frame with header
     */
    static void createBME280Frame(uint8_t buff[], char flags[], uint32_t time,
                                   int32_t temp, int32_t humidity, int32_t pressure, int32_t altitude);

    /**
     * Main packer function for the CCS811 data frame.
     * @param flags
     *      Header flags
     * @param time
     *      Time in seconds (Epoch)
     * @param co2
     *      CO2 in ppm
     * @param TVOC
     *      TVOC in ppm
     * @return
     *      A fully complete data frame with header
     */
    static void createCCS811Frame(uint8_t buff[], char flags[], uint32_t time,
                                   int16_t co2, int16_t TVOC);

    /**
     * Unpack function for the LSM9DS1 data frame
     * @param buff
     *      A array containing a packed LSM9DS1 packet
     * @return
     *      A LSM9DS1 packet
     */
    static LSM9DS1_Packet unpack_LSM9DS1(const uint8_t buff[]);

    /**
     * Unpack function for the BME280 packet
     * @param buff
     *      A array containing a packed BME280 packet
     * @return
     *      A BME280 packet
     */
    static BME280_Packet unpack_BME280(const uint8_t buff[]);

    /**
     * Unpack function for the CCS811 packet
     * @param buff
     *      A array containing a packed CCS811 packet
     * @return
     *      A CCS811 packet
     */
    //static CCS811_Packet unpack_CCS811(const uint8_t buff[]);

private:
    /**
     * Creates a header using the provided values below.
     * @param type
     *      Type of data frame
     * @param flags
     *      Flags passes as an array
     * @param time
     *      Time in seconds
     * @return
     *      A null terminated string
     */
    static void createHeader(uint8_t buff[], DataFrameType type, const char *flags, uint32_t time);

    /**
     * Unpacks function for a hyperion header.
     * @param buff
     *      Array of header data values
     * @return
     *      A packed header struct
     */
    static unpk_header unpack_header(const uint8_t buff[]);

    /**
     * Unpack function for the LSM9DS1 data segment
     * @param buff
     *      Array of LSM9DS1 data values
     * @return
     *      A packed LSM9DS1 Data struct
     */
    static LSM9DS1_Data unpack_LSM9DS1_Data(const uint8_t buff[]);

    /**
     * Unpack function for the BME280 data segment
     * @param buff
     *      Array of BME280 data values
     * @return
     *      A packed BME280 Data struct
     */
    static BME280_Data unpack_BME280_Data(const uint8_t buff[]);


    /**
     * Unpack function for the CCS811 data segment
     * @param buff
     *      Array of CCS811 data values
     * @return
     *      A packed CCS811 Data struct
     */
    //static CCS811_Data unpack_CCS811_Data(const uint8_t buff[]);
};
#endif
