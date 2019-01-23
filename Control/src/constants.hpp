#pragma once
#include <cinttypes>
#include <unistd.h>
#define RUNNING_ON_RASPBIAN 0

namespace constants {
    //time between updates in microseconds
    const useconds_t TIME_DELTA = 50*1000;
	// SPI protocol frequency to be used. 4MHz (clock_div=64) is as fast as the ATmega328P can clock in data
	const uint32_t SPI_FREQ = 4000000;
    const uint8_t SPI_PORT = 0;
        const uint16_t spiBufferSize = 20;
	const uint16_t uiBufferSize = 22;
    const char* FIFO_NAME = "controllerToUI";
}
