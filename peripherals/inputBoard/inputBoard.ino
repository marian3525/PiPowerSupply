/**
 * Created by marian3525.
 * Code for the slave board which takes inputs from the buttons, ENCODERODERoders and temperature sensors, packs them in a byte array and sends them over SPI to the Raspberry PI master
 * Arduino Nano (ATmega328P, 16MHz, 3.3V, 4MHz SPI speed)
 * 
 * Sampling cycle:
 * If it isn't the time to send the buffer, check to see if the ENCODERODERoders have rotated and increment the counters
 *                                          check if any buttons have been pressed and save a 255 in the buffer at their status position if they have
*/
#include <Encoder.h>
#include "constants_inputBoard.h"

Encoder encAV(pin::ENCODER_VOLTAGE_A_P1, pin::ENCODER_VOLTAGE_A_P2);
Encoder encAA(pin::ENCODER_CURRENT_A_P1, pin:: ENCODER_CURRENT_A_P2);
Encoder encBV(pin::ENCODER_VOLTAGE_B_P1, pin::ENCODER_VOLTAGE_B_P1);
Encoder encBA(pin::ENCODER_CURRENT_B_P1, pin::ENCODER_CURRENT_B_P2);

const byte bufferSize=13;
uint8_t* SPIsend;
uint8_t* SPIreceive;
bool usingMISO=false;

// timing variables
uint32_t lastUpdate=0;
uint8_t updateDelta=16;  // from master
bool packetReady = false;

//temperature map. temp[temperature] = resistance in kOhms;
const float temp[101] = {159.9, 152.2, 144.9, 138, 131.4, 125.2, 119.4, 113.8, 108.5, 103.5,
						98,   942,   89.9,  85.8, 81.9, 78.2,  74.7,  71.4,  68.2,  65.2,
						62.3, 59.6,  57,    54.5, 52.2, 50,    47.8,  45.8,  43.8,  42,  40.2,
						38.5, 36.9,  35.4,  33.9, 32.6, 31.2,  29.9,  28.7,  27.6,  26.5,
						25.4, 24.4,  23.4,  22.5, 21.6, 20.8,  20,    19.2,  18.4,  17.9,
						17.1, 16.4,  15.8,  15.2, 14.6, 14.1,  13.5,  13,    12.6,  12.1, 
						11.7, 11.2,  10.8,  10.4, 10,   9.7,   9.4,    9,     8.7,   8.4, 
						8.1,   7.8,   7.5,   7.3,  7,   6.8,   6.5,   6.3,    6.1,   5.9, 
						5.7,   5.5,   5.3,   5.1,  5,   4.8,   4.7,   4.5,    4.4,   4.2, 
						4.1,   3.9,   3.8,   3.7, 3.6,  3.5,   3.4,   3.3,    3.2,   3.1};

inline void SPITransferVector() {
  // 2us per byte
  for(uint8_t i=0; i< bufferSize; i++) {
      SPDR = SPIsend[i];
      while(!(SPSR & (1 << SPIF))){};
      SPIreceive[i] = SPDR;
  }

  updateDelta = SPIreceive[0];
}

inline uint8_t getChecksum() {
    uint8_t chk=0;
    for(uint8_t i=0; i<bufferSize-1; i++)
        chk += SPIsend[i];
    return chk;
}

int8_t getTempFromTable(float resistance) {
	/*
		Find the closest aproximation of the temperature from the temp map
		resistance given in Ohms
	*/
	
  	resistance /= 1000;
  	uint8_t middle, left = 0, right = 100, idx=0;
	  float min_delta;
	  float delta;
    
  	while (true) {
  		  middle = (left + right) / 2;
  
  		  if (left - right <= 4) {
    			  min_delta = 99;
    			  idx=0;
            
    			  for (uint8_t i = left; i <= right; i++) {
                delta = abs(resistance - temp[i]);
      				  if (delta < min_delta) {
        					  min_delta = delta;
        					  idx = i;
      				  }
    			  }
    			  return idx;
  		  }
  		  else if (temp[middle] >= resistance) {
    			  //search in the right half
    			  left = middle;
  		  }
  		  else {
    			  //search in the left half
    			  right = middle;
  		  }
  	 }
}

inline int8_t getTemperaturePSU() {
  	const uint8_t R=10;            // pulldown in kOhms
    uint8_t Rt=0;

    Rt = R * 1023 / analogRead(pin::PSU_TEMPERATURE_SENSE) - R;
    
  	return getTempFromTable(Rt);
}
inline int8_t getTemperatureCHA() {
  	const uint8_t R=10;            // pulldown in kOhms
    uint8_t Rt=0;

    Rt = R * 1023 / analogRead(pin::CHANNELA_TEMPERATURE_SENSE) - R;
    
    return getTempFromTable(Rt);
}
inline int8_t getTemperatureCHB() {
  	const uint8_t R=10;            // pulldown in kOhms
    uint8_t Rt=0;

    Rt = R * 1023 / analogRead(pin::CHANNELB_TEMPERATURE_SENSE) - R;
    
    return getTempFromTable(Rt);
}

void sampleInput() {
    // keep a 1 if the button has already been pressed this cycle (also achieves soft debouncing)
    // will be set back to 0 after the data has been sent
    SPIsend[0] = updateDelta;
    for(uint8_t i=1; i<bufferSize; i++)
        SPIsend[i] = i+200;
    /*
  	SPIsend[0] = digitalRead(pin::BUTTON_NEXT) < SPIsend[0] ? 0xFF: 0x00;
  	SPIsend[1] = digitalRead(pin::BUTTON_PREVIOUS) < SPIsend[1] ? 0xFF: 0x00;
  	SPIsend[2] = digitalRead(pin::BUTTON_SELECT) < SPIsend[2] ? 0xFF : 0x00;
  	SPIsend[3] = digitalRead(pin::BUTTON_AUX) < SPIsend[3]? 0xFF : 0x00;

    SPIsend[4] = getTemperaturePSU();
    SPIsend[5] = getTemperatureCHA();
    SPIsend[6] = getTemperatureCHB();

    SPIsend[7] = encAV.read();
    SPIsend[8] = encAA. SPIreceive[0];read();
    SPIsend[9] = encBV.read();
    SPIsend[10] = encBA.read();
    
  	SPIsend[11] = digitalRead(pin::MODE) ? 0xFF : 0x00;      // this button is a latching one, so it will hold its state
    */
}

void processResponse() {
	updateDelta = SPIreceive[0];
}

void setup() {
  	SPIsend = new uint8_t[bufferSize];
	  SPIreceive = new uint8_t[bufferSize];
    
  	pinMode(pin::BUTTON_NEXT, INPUT_PULLUP);
  	pinMode(pin::BUTTON_PREVIOUS, INPUT_PULLUP);
  	pinMode(pin::BUTTON_SELECT, INPUT_PULLUP);
  	pinMode(pin::BUTTON_AUX, INPUT_PULLUP);
  	
  	pinMode(pin::PSU_TEMPERATURE_SENSE, INPUT);
  	pinMode(pin::CHANNELA_TEMPERATURE_SENSE, INPUT);
  	pinMode(pin::CHANNELB_TEMPERATURE_SENSE, INPUT);
  	pinMode(pin::MODE, INPUT);

    pinMode(3, OUTPUT);
    digitalWrite(3, LOW);

    pinMode(SCK, INPUT);                    // clock generated by master. Should be at most 4MHz or most data will be lost.
    pinMode(MOSI, INPUT);                   // MOSI should be high impedance when not talked to
    pinMode(MISO, INPUT);                   // MISO is always input
    pinMode(SS, INPUT);                     // brought LOW when the transfer is started
    
    SPCR |= _BV(SPE);                       //temperature enable SPI as slave
}
	

void loop() {
    switch (digitalRead(SS)) {
        case HIGH: {
            pinMode(MISO, INPUT);               // MISO set to high impedance so it doesn't interfere with other SPI comms
            usingMISO = false;
            
            // check our time.
            if(millis() - lastUpdate > updateDelta - 5 && packetReady == false) {      // 5ms before the update, prepare the packet
                // put data in its place
                // compute the checksum and wait for the master to bring SS low
                SPIsend[bufferSize-1] = getChecksum();                
                packetReady = true;
            }
            else {
                // Not time to prepare the packet, check if there is any more work to do
                if(packetReady == false) {
                    // ...get more data samples etc
                    sampleInput();
                }
                // if the packet is ready, loop again, there is nothing to do but wait for the update
            }
            
            break;
        }
        case LOW: {									// SS line low, slave now selected, time to talk to the Pi
            if (!usingMISO) {						// if we weren't using MISO as output in the prev. loop, set it to low Z
                pinMode(MISO, OUTPUT);				// MISO configured to output data
                usingMISO = true;                   // remember that we're used it
            }
            
						// send the data in the buffer and receive the updated status
            SPITransferVector();
            //process the response  
            // reinit for the next cycles of updates in the HIGH branch
            packetReady = false;
            lastUpdate = millis();
            
            break;
        }
    }
}
