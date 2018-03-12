/**
 * File: IREC-Hyperion_Protocol.h
 * Organization: RIT Space Exploration
 * Description:
 *      Protocol to be used on the Hyperion payload.
 */

#ifndef IREC_HYPERION_PROTOCOL_LIBRARY_H
#define IREC_HYPERION_PROTOCOL_LIBRARY_H

enum DataFrameType{
    NULLDATA,
    LSM9DS1,
    BME280,
    CCS811,
    PFSL
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
    static uint8_t *createLSM9DS1Frame(char *flags, uint32_t time,
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
    static uint8_t *createBME280Frame(char flags[], uint32_t time,
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
    static uint8_t *createCCS811Frame(char flags[], uint32_t time,
                                   int16_t co2, int16_t TVOC);

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
    static u_int8_t *createHeader(DataFrameType type, const char *flags, uint32_t time);
};
#endif
