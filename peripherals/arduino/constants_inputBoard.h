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
	const uint8_t ENCODER_VOLTAGE_A_P1 = 0;             //
	const uint8_t ENCODER_VOLTAGE_A_P2 = 1;             //
	//should be interrupt
	const uint8_t ENCODER_CURRENT_A_P1 = 2;             //
	const uint8_t ENCODER_CURRENT_A_P2 = 3;             //
	//should be interrupt
	const uint8_t ENCODER_VOLTAGE_B_P1 = 4;             //
	const uint8_t ENCODER_VOLTAGE_B_P2 = 5;             //
	//should be interrupt
	const uint8_t ENCODER_CURRENT_B_P1 = 6;             //
	const uint8_t ENCODER_CURRENT_B_P2 = 7;             //DELEGA
	
	const uint8_t CURRENT_MODE = 8;                    

	//PREDEFINED MISO, MOSI, SCLK
	const uint8_t MOSI = 10;
	const uint8_t MISO = 11;
	const uint8_t CLK = 12;
	const uint8_t SS = 13;

}
