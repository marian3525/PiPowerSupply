#pragma once
#include <cinttypes>

namespace pin {
	// power sense
    const uint8_t PWR_GOOD = 0;

	//converter pins - DIGITAL OUT
	const uint8_t CS_PIN_ADC = 0;
	const uint8_t CS_PIN_DAC_A = 0;
	const uint8_t CS_PIN_DAC_B = 0;

	//PREDEFINED MISO, MOSI, SCLK: main board
	const uint8_t MOSI = 0;
	const uint8_t MISO = 0;
	const uint8_t CLK = 0;

	//DIGITAL OUT
	const uint8_t LATCH_DAC_A = 0;
	const uint8_t LATCH_DAC_B = 0;
	const uint8_t INPUT_BOARD_CS = 0;

	//PWM OUT
	//pin output to control the SMPS PWM
	const uint8_t SMPS_CONTROL = 0;
	const uint8_t BUZZER_PIN = 0;

	// Chip select pin for the button/encoder/temperature slave input board
	const uint8_t CS_INPUT_BOARD = 1;
}
