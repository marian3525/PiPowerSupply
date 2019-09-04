void setup (void)
{
    // have to send on master in, *slave out*
    pinMode(MISO, OUTPUT);

    // turn on SPI in slave mode
    SPCR |= _BV(SPE);

    // turn on interrupts
    SPCR |= _BV(SPIE);

}

// SPI interrupt routine
ISR(SPI_STC_vect)
{
    byte c = SPDR;
    SPDR = 0xAB;

}

void loop (void){
    SPDR = 0xAD;
}
