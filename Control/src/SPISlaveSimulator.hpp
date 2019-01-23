#pragma once
#include <cstdio>
#include <cinttypes>

class SPISlaveSimulator {
public:
    void simADCWiringPiSpiRW(int spiPort, uint8_t* data, int len);
    void simDAC1WiringPiSpiRW(int spiPort, uint8_t* data, int len);
    void simDAC2WiringPiSpiRW(int spiPort, uint8_t* data, int len);

    void simInputBoardWiringPiSpiRW(int spiPort, uint8_t* data, int len);
    void update();

private:
    uint16_t chAV = 3200;
    uint16_t chAA = 128;
    uint16_t chBV = 0;
    uint16_t chBA = 0;
    uint16_t* inputBoardData;
};
