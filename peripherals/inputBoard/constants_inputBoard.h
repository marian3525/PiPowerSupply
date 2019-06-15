/**
 * 	Shared header between the main board and the input board binaries.
 * 	Entries commented with '' will be used by the input board only
 */
namespace pin {
	// buttons
	const uint8_t BUTTON_NEXT = A6;						//DIGIAL IN: 
	const uint8_t BUTTON_PREVIOUS = A5;					//DIGITAL IN: 
	const uint8_t BUTTON_SELECT = A4;					//DIGITAL IN: 
	const uint8_t BUTTON_AUX = A3;						//DIGITAL IN: 
	
	//ANALOG IN
	const uint8_t PSU_TEMPERATURE_SENSE = A0;  //
	const uint8_t CHANNELA_TEMPERATURE_SENSE = A1;      //
	const uint8_t CHANNELB_TEMPERATURE_SENSE = A2;      //
	
	//DIGITAL IN - INTERRUPTS
	//should be interrupt	(at least one pin per Encoder)
	const uint8_t ENCODER_VOLTAGE_A_P1 = 2;             //
	const uint8_t ENCODER_VOLTAGE_A_P2 = 4;             //
	//should be interrupt
	const uint8_t ENCODER_CURRENT_A_P1 = 0;             //
	const uint8_t ENCODER_CURRENT_A_P2 = 0;             //
	//should be interrupt
	const uint8_t ENCODER_VOLTAGE_B_P1 = 0;             //
	const uint8_t ENCODER_VOLTAGE_B_P2 = 0;             //
	//should be interrupt
	const uint8_t ENCODER_CURRENT_B_P1 = 0;             //
	const uint8_t ENCODER_CURRENT_B_P2 = 0;             //DELEGA
	
	const uint8_t MODE = 0;
	
	//PREDEFINED MISO, MOSI, SCLK
  const uint8_t SS = 10;
	const uint8_t MOSI = 11;
	const uint8_t MISO = 12;
	const uint8_t CLK = 13;
}
