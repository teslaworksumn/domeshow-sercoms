#ifndef DSCom_H
#define DSCom_H
#define LIBRARY_VERSION_DSCOM_H   "0.3.2-alpha"

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <crc16.h>

#define DSCOM_STATE_READY     0
#define DSCOM_STATE_READING   1
#define DSCOM_STATE_APPLY     2
#define DSCOM_MAGIC_LENGTH    4

#define DSCOM_MAX_LENGTH      128

/*
 * Use the following defines to control debug info:
 * #define DSCOM_DEBUG
 * #define DSCOM_DEBUG_1
 * #define DSCOM_DEBUG_2
 * WARNING: DO NOT ATTEMPT TO DEBUG AND WRITE!
 */

//#define DSCOM_DEBUG
//#define DSCOM_DEBUG_1
//#define DSCOM_DEBUG_2

class DSCom {
public:
    /**
     * Default constructor.  Uses the primary serial port by default
     * Do not use with a native USB port.  Things break.
     */
    /* TODO: Broken on Teensy: DO NOT USE
    inline DSCom() {
        s = &Serial;
    }
    */
    /**
     * Constructor, specifying a particular serial port
     */
    inline DSCom(HardwareSerial &port) {
        s = &port;
    };

    /**
     * Call this in a loop to read and process the data from the serial port
     */
    void read();

    /**
     * Returns the last array of integers received by the library.
     *  WARNING: do not call before the first packet has been received.
     *  That may have unexpected results.
     */
    uint8_t* getData();

    /**
     * Returns the number of items returned by getData() 
     */
    inline uint16_t getDataLen() {
        return data_len;
    }

    /**
     * Returns true if a packet has been received since getData() was last
     *  called.
     */
    inline bool isUpdated() {
        return updated;
    }

    /**
     * writes data of length len to the serial port
     */
    uint16_t write(uint8_t* data, uint16_t len);

    /**
     * Calls the serial port's flush() function (convenience function)
     */
    inline void flush() {
        s->flush();
    }

private:
    HardwareSerial* s;
    bool messagewalk = false;  // Prevents spamming Serial monitor
    const uint8_t magic[DSCOM_MAGIC_LENGTH] = {0xde, 0xad, 0xbe, 0xef};
    uint8_t magic_status = 0;
    uint8_t state = DSCOM_STATE_READY;

    uint8_t* data;
    uint8_t* new_data;
    bool updated = false;

    uint16_t data_len = 0;
    uint16_t new_data_len = 0;

    uint16_t getTwoBytesSerial();
    void splitTwoBytes(uint16_t in, uint8_t &out_high, uint8_t &out_low);
    uint16_t readData(uint16_t len);

    crc16 crc;
};
#endif