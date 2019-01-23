#pragma once
#include <cinttypes>
#include "SPISlaveSimulator.hpp"

struct ADCConfig {
    uint8_t mode;   // 1:single or 0:differential
};


class MCP3204
{
public:
    MCP3204(ADCConfig config): mode{config.mode} {
        spiBuffer = new uint8_t[3];
        sim = new SPISlaveSimulator();
    };
    uint16_t read(uint8_t channel);
private:
    uint8_t mode;
    uint8_t* spiBuffer;
    SPISlaveSimulator* sim;
};
