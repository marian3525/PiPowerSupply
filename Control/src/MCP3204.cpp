#include "MCP3204.hpp"
//#include "constants.hpp"
uint16_t MCP3204::read(uint8_t channel) {
    /**
     * bit layout for the buffer:
     * 0 0 0 0 0 1 mode 0    ch1 ch2 dnc(x6) dnc(x8) : send
     *  ?(x8)   ?(x3) 0 B11 B10 B9 B8        B7 ... B0
     */
    uint16_t value = 0;
    uint8_t builder = 0;

    //config bits: mode and channel
    builder |= 1 << 2;        // start bit
    builder |= mode << 1;     // mode: 1 for single, 0 is differential
    spiBuffer[0] = builder;   // first byte done

    builder = 0;
    builder |= channel << 6;    // first 2 bits determine the channel
    spiBuffer[1] = builder;

#if RUNNING_ON_RASPBIAN == 1
    wiringPiSPIDataRW(0, spiBuffer, 3);
#else
    sim->simADCWiringPiSpiRW(0, spiBuffer, 3);
#endif
    // extract the value from the received data in byte 2 and 3 of the buffer
    value = static_cast<uint8_t>((spiBuffer[1] & 0x0F) << 8);     // extract the 4 MSB from the buffer and put them into place in value
    value |= spiBuffer[2];      // place the 8 LSB
    value &= 0x0FFF;            // keep 12 bits
    return value;
}
