#pragma once
#include <cinttypes>
#include "SPISlaveSimulator.hpp"

struct DACConfig {
    uint8_t buffered;
    uint8_t gain;
    uint8_t shutdown;
};


class MCP4922
{
public:
    MCP4922(DACConfig config): buffered{config.buffered}, gain{config.gain}, shutdown{config.shutdown} {
        spiBuffer = new uint8_t[2];
        sim = new SPISlaveSimulator();
    };
    void write(uint8_t channel, uint16_t value);

private:
    uint8_t buffered;
    uint8_t gain;
    uint8_t shutdown;
    uint8_t* spiBuffer;
    SPISlaveSimulator* sim;
};
