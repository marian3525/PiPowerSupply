#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "MCP4922.hpp"

void MCP4922::write(uint8_t channel, uint16_t value) {
    /**
      Write the 12-bit value to the provided channel
      Bit layout in the SPI buffer:
      (channel) (buffered) (gain) (shutdown) D11 ... D0
      */
    uint8_t builder = 0;    // aux variable in which the 2 bytes will be built
    uint8_t buffered = 0;
    uint8_t gain = 1;
    uint8_t shutdown = 1;
    uint8_t spiBuffer[2];

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
	
	// send the data
    digitalWrite(7, LOW);
    wiringPiSPIDataRW(0, spiBuffer, 2);
    digitalWrite(7, HIGH);
}
