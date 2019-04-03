#include "Controller.hpp"
#include "pin.hpp"
#include "constants.hpp"

/**
 * @brief Controller::Controller Creates a controller and opens (RW) a named pipe to talk to the UI
 */
Controller::Controller()
{
    // prevent process memory swapping
    struct sched_param sp{};
    memset(&sp, 0, sizeof(sp));
    sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
    sched_setscheduler(0, SCHED_FIFO, &sp);
    mlockall(MCL_CURRENT | MCL_FUTURE);

    IOinit();
    SPIinit();

    // create a pipe to send data to the UI
    mkfifo(constants::FIFO_NAME, 0666);
    descriptorW = open(constants::FIFO_NAME, O_WRONLY);
    descriptorR = open(constants::FIFO_NAME, O_RDONLY);
}

Controller::~Controller() {
    close(descriptorW);
    close(descriptorR);
}

/**
 * @brief Controller::start Must be called to start running (read and write data)
 */
void Controller::start() {
    bool powered=false;
    std::cout<<"waiting for power\n";

    while(!powered) {
#if RUNNING_ON_RASPBIAN == 1
        powered = digitalRead(pin::PWR_GOOD);
#endif
        //usleep(1);
    }

    std::cout<<"Running...\n";

    while(powered) {
	std::cout<<"Looping\n";
        syncWithHardware();
        syncWithUI();
#if RUNNING_ON_RASPBIAN ==1
        powered = digitalRead(pin::PWR_GOOD);
#endif
        usleep(constants::TIME_DELTA);
    }
}

/**
 * @brief Controller::IOinit Configure the IO of the Pi
 */
void Controller::IOinit() {
    // raspbian specific
#if RUNNING_ON_RASPBIAN == 1
    // SPI CS pins for the devices on the bus
    pinMode(pin::CS_PIN_ADC, OUTPUT);
    pinMode(pin::CS_PIN_DAC_A, OUTPUT);
    pinMode(pin::CS_PIN_DAC_B, OUTPUT);
    pinMode(pin::LATCH_DAC_A, OUTPUT);
    pinMode(pin::LATCH_DAC_B, OUTPUT);
    pinMode(pin::CS_INPUT_BOARD, OUTPUT);

    pinMode(pin::SMPS_CONTROL, OUTPUT);
    pinMode(pin::BUZZER_PIN, OUTPUT);
#endif
}

/**
 * @brief Controller::SPIinit Start SPI on port 0 at SPI_FREQ Hz
 */
void Controller::SPIinit() {
    // SPI library init
#if RUNNING_ON_RASPBIAN == 1
    if(wiringPiSPISetup(0, constants::SPI_FREQ) == -1) {
        std::cout<<"Could not init SPI";
    }
    // unselect all chips on the bus
    digitalWrite(pin::CS_INPUT_BOARD, HIGH);
#endif
}

/**
 * @brief Controller::readChannelStatesFromADCs Read digital values from the 2 channels, each with volts and amps
 * @return
 */
pair<ChannelState, ChannelState> Controller::readChannelStatesFromADCs() {

    uint16_t AV, AA, BV, BA;
    AV = converter.read(Channel::voltageA);
    AA = converter.read(Channel::currentA);

    BV = converter.read(Channel::voltageB);
    BA = converter.read(Channel::currentB);

    return {{{AV, AA}, 0}, {{BV, BA}, 0}};
}

/**
 * @brief Controller::writeStatesToDACs: write the values stored in the parameter to the DACs for the 2 channels
 * @param outState: stores the values to be written
 */
void Controller::writeStatesToDACs(pair<ChannelState, ChannelState> outState) {
    converter.write(Channel::voltageA, outState.first.VAvalues.voltage);
    converter.write(Channel::currentA, outState.first.VAvalues.current);

    converter.write(Channel::voltageB, outState.second.VAvalues.voltage);
    converter.write(Channel::currentB, outState.second.VAvalues.current);
}

/**
 * @brief Controller::syncWithHardware Does most of the work to keep the knobs in sync with the configuration
 *
 */
void Controller::syncWithHardware() {
    auto inputStates = inputBoard.readStateFromInputBoard();
    auto channelStates = readChannelStatesFromADCs();

    VAValues freshA{0, 0}, freshB{0, 0};

    freshA.voltage = converter.read(Channel::voltageA);
    freshA.current = converter.read(Channel::currentA);

    freshB.voltage = converter.read(Channel::voltageB);
    freshB.current = converter.read(Channel::currentB);

    writeStatesToDACs(channelStates);
    outputBoard.writeStateToOutputBoard(outputStates);
}
/**
 * @brief Controller::syncWithUI Write all the required data to the UI
 */
void Controller::syncWithUI() {
    /**
     * Send uiBuffer through the FIFO to the ui
     */
    uint16_t sentSize = 0;

    uint8_t receivedSize = 0;

    while(sentSize < uiSendBufferSize) {
        // send chucks until complete
        sentSize += write(descriptorW, uiSendBuffer, uiSendBufferSize);
    }

    while(receivedSize < uiReceiveBufferSize) {
        receivedSize += read(descriptorR, uiReceiveBuffer, uiReceiveBufferSize);
    }
}

/**
 * @brief Controller::addEncoderDeltaToVA: add the number of pulses from the encoders to the prev. value read
 * @return the updated values computed
 */
pair<VAValues, VAValues> Controller::addEncoderDeltaToVA() const {
    VAValues newValuesA{0, 0};
    VAValues newValuesB{0, 0};

    // add the number of encoder pulses registered in the prev. cycle to the current values
    newValuesA.voltage += encoderState.encoderAV;
    newValuesA.current += encoderState.encoderAA;

    newValuesB.voltage += encoderState.encoderBV;
    newValuesB.current += encoderState.encoderBA;

    return {newValuesA, newValuesB};
}

pair<ChannelState, ChannelState> Controller::getChannelsState() const {
    return channelStates;
}

vector<pair<ButtonName, bool>> Controller::getButtonStates() const {
    return buttonStates;
}

EncoderState Controller::getEncoderDeltas() const {
    return encoderState;
}
