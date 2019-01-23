
#include "SPISlaveSimulator.hpp"

void SPISlaveSimulator::simADCWiringPiSpiRW(int spiPort, uint8_t *data, int len) {
#if RUNNING_ON_RASPBIAN == 0
    if(len != 3) {
    printf("Invalid number of bytes sent");
    }
    else {
    data[0] = 0x00;
    uint16_t value = 0;
    // find the channel we're reading from
    uint8_t channel = static_cast<uint8_t>((data[1] >> 6) & 0x03);
    switch(channel) {
        case 0x00:
            value = 3000;
        break;
        case 0x01:
            value = chAA;
        break;
        case 0x03:
            value = chBV;
        break;
        case 0x04:
            value = chBA;
        break;
    }
    data[1] = static_cast<uint8_t>(value >> 8);	// keep the 4 MSB of the 12 but value
    data[2] = static_cast<uint8_t>(value & 0x00FF);		// keep the 8 LSB of the 12 bit value
    }
#endif
}

void SPISlaveSimulator::simDAC1WiringPiSpiRW(int spiPort, uint8_t *data, int len) {
    /**
      Write the to chXY the value extracted from the data bytes
      */
#if RUNNING_ON_RASPBIAN == 0

    if(len != 2) {
    printf("Invalid length");
    }
    else {
    // working, extracts the bits properly
    uint8_t channel = static_cast<uint8_t>((data[0] >> 7) & 0x01);
    uint8_t buffered = static_cast<uint8_t>((data[0] >> 6) & 0x01);
    uint8_t gain = static_cast<uint8_t>((data[0] >> 5) & 0x01);
    uint8_t shdn = static_cast<uint8_t>((data[0] >> 4) & 0x01);

    if(channel == 0) {
       chAV = static_cast<uint16_t>((data[0] & 0x0F) & 0x00FF);
       chAV = static_cast<uint16_t>(chAV << 8);
       chAV = chAV | static_cast<uint8_t>(data[1]);
    }
    }
#endif
}

void SPISlaveSimulator::simDAC2WiringPiSpiRW(int spiPort, uint8_t *data, int len) {
    /**
      Write the to chXY the value extracted from the data bytes
      */
#if RUNNING_ON_RASPBIAN == 0

    if(len != 2) {
        printf("Invalid length");
    }
    else {
        // working, extracts the bits properly
        uint8_t channel = (data[0]>>7) & 0x01;
        uint8_t buffered = (data[0]>>6) & 0x01;
        uint8_t gain = (data[0]>>5) & 0x01;
        uint8_t shdn = (data[0]>>4) & 0x01;

        if(channel == 0) {
            chAV = data[0] & 0x0F;
            chAV = static_cast<uint16_t>(chAV << 8);
            chAV = chAV | static_cast<uint16_t>(data[1]);
        }
    }
#endif
}


void SPISlaveSimulator::simInputBoardWiringPiSpiRW(int spiPort, uint8_t *data, int len) {
#if RUNNING_ON_RASPBIAN == 0

    for(unsigned char i = 0; i<len; i++) {
	data[i] = i;
    }
#endif
}

void SPISlaveSimulator::update() {
#if RUNNING_ON_RASPBIAN == 0

    chAV++;
    chAA++;
    chBV++;
    chBA++;

#endif
}
