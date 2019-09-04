#pragma once
#include "../utils/MCP3204.hpp"
#include "../utils/MCP4922.hpp"

enum class Channel {voltageA, currentA, voltageB, currentB};

class Converters
{
public:
    Converters();
    uint16_t read(Channel fromChannel);
    void write(Channel toChannel, uint16_t value);

private:
    MCP3204 adc = MCP3204(ADCConfig{1});            // mode: single:1, differential:0
    MCP4922 dac1 = MCP4922(DACConfig{1, 1, 1});	    // buffered:1, gain=1:1, shutdown: active low:1
    MCP4922 dac2 = MCP4922(DACConfig{1, 1, 1});	    // buffered:1, gain=1:1, shutdown: active low:1
};
