#define CS 10

byte* buffer;
int n=1;
bool usingMISO = false;
bool flag=true;

inline byte SPItransfer(byte value) {
  
    SPDR = value;
    while (!(SPSR & (1 << SPIF))){};
    return SPDR;
}

inline void SPI_RW(byte* buffer, int len) {
    for(int i=0; i<len; i++) {
        SPItransfer(buffer[i]+1);
    }
}

void setup() {
    buffer = new byte[n];

    for(int i=0; i<n; i++)
        buffer[i] = 'a' + i;

    pinMode(SCK, INPUT);
    pinMode(MOSI, INPUT);                   // MOSI should be high impedance when not talked to
    pinMode(MISO, INPUT);
    pinMode(CS, INPUT);
    
    SPCR |= _BV(SPE);
}

void loop() {
    switch (digitalRead(CS)) { 
        case HIGH: {                        // CS line high, not selected
            if (usingMISO) {
                
                pinMode(MISO, INPUT);       // MISO set to high impedance
                usingMISO = false;
            }
            break;
        }
        case LOW: {                         // CS line low, slave new selected
            if (!usingMISO) {
                
                pinMode(MISO, OUTPUT);      // MISO configured to output data
                usingMISO = true;
            }
            //SPI_RW(buffer, n);
            SPItransfer(123);
            break;
        }
    }
}
