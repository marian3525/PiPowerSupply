#pragma once
#include <cinttypes>

struct DACConfig {
    uint8_t buffered;
    uint8_t gain;
    uint8_t shutdown;
};


class MCP4922
{
public:
    /*
     * Default working state: gain: 1, shutdown: 1,buffered: 0
     */
    MCP4922(DACConfig config): buffered{config.buffered}, gain{config.gain}, shutdown{config.shutdown} {};
    void write(uint8_t channel, uint16_t value);

private:
    uint8_t buffered;
    uint8_t gain;
    uint8_t shutdown;
};
