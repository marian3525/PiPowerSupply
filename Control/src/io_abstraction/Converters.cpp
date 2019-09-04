#include "Converters.hpp"

Converters::Converters() {

}

uint16_t Converters::read(Channel fromChannel) {
    switch(fromChannel) {

        case Channel::voltageA:
        return adc.read(0);

        case Channel::currentA:
        return adc.read(1);

        case Channel::voltageB:
        return adc.read(2);

        case Channel::currentB:
        return adc.read(3);
    }
    return 0;
}

void Converters::write(Channel toChannel, uint16_t value) {
    switch(toChannel) {

    case Channel::voltageA:
        dac1.write(0, value);
        break;

    case Channel::currentA:
        dac1.write(1, value);
        break;

    case Channel::voltageB:
        dac2.write(0, value);
        break;

    case Channel::currentB:
        dac2.write(1, value);
        break;
    }
}
