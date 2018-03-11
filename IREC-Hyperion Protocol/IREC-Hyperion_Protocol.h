/**
 * File: IREC-Hyperion_Protocol.h
 * Organization: RIT Space Exploration
 * Description:
 *      Protocol to be used on the Hyperion payload.
 */

#ifndef IREC_HYPERION_PROTOCOL_LIBRARY_H
#define IREC_HYPERION_PROTOCOL_LIBRARY_H

/**
 * Main class for IREC-HYPERION_PROTOCOL
 */
class IRECHYPERP {
public:

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
    static char *createHeader(int type, char flags[], long time);

};
#endif