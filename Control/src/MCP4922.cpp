#include "MCP4922.hpp"
//#include "constants.hpp"
#include "wiringPiSPI.h"

void MCP4922::write(uint8_t channel, uint16_t value) {
    /**
      Write the 12-bit value to the provided channel
      Bit layout in the SPI buffer:
      (channel) (buffered) (gain) (shotdown) D11 ... D0
      */
    uint8_t builder = 0;    // build the first byte

    // config bits
    builder |= static_cast<uint8_t>(channel << 7);       // set the channel bit
    builder |= static_cast<uint8_t>(buffered << 6);      // buffered bit
    builder |= static_cast<uint8_t>(gain << 5);          // gain bit
    builder |= static_cast<uint8_t>(shutdown << 4);      // shutdown bit

    // top 4 MSB bits form the value
    builder |= static_cast<uint8_t>(value >> 8);
    // done building, save the data in the first byte in the buffer
    spiBuffer[0] = builder;

    // the 8 LSB bits from value will be put in the other 2nd byte
    spiBuffer[1] = static_cast<uint8_t>(value & 0x00FF);
#if RUNNING_ON_RASPBIAN == 1
    wiringPiSPIDataRW(0, spiBuffer, 2);
#else
    sim->simDAC1WiringPiSpiRW(0, spiBuffer, 2);
#endif
}
