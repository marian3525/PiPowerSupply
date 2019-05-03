#pragma once
#include <cinttypes>

struct ADCConfig {
    uint8_t mode;   // 1:single or 0:differential
};


class MCP3204
{
public:
    MCP3204(ADCConfig config): mode{config.mode} {};
    uint16_t read(uint8_t channel);
private:
    uint8_t mode;
};
