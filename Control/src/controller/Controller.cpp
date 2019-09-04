#include "Controller.hpp"
#include "../utils/pin.hpp"
#include "../utils/constants.hpp"
#include "../utils/IPCHandler.hpp"

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

    ipcHandler = new IPCHandler{};
}

Controller::~Controller() {
    delete ipcHandler;
}

/**
 * @brief Controller::start Must be called to start running (read and write data)
 */
void Controller::start() {
    bool powered=false;

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
pair<VAValues, VAValues> Controller::readChannelStatesFromADCs() {

    uint16_t AV, AA, BV, BA;
    AV = converter.read(Channel::voltageA);
    AA = converter.read(Channel::currentA);

    BV = converter.read(Channel::voltageB);
    BA = converter.read(Channel::currentB);

    return {{AV, AA}, {BV, BA}};
}

/**
 * @brief Controller::writeStatesToDACs: write the values stored in the parameter to the DACs for the 2 channels
 * @param outState: stores the values to be written
 */
void Controller::writeValuesToDACs(pair<VAValues, VAValues> outValues) {
    converter.write(Channel::voltageA, outValues.first.voltage);
    converter.write(Channel::currentA, outValues.first.current);

    converter.write(Channel::voltageB, outValues.second.voltage);
    converter.write(Channel::currentB, outValues.second.current);
}

/**
 * @brief Controller::syncWithHardware Does most of the work to keep the knobs in sync with the configuration
 *
 */
void Controller::syncWithHardware() {
    // button and encoder states
    auto inputStates = inputBoard.readStateFromInputBoard();

    // digital values of the voltage and current for both channels
    auto channelValues = readChannelStatesFromADCs();

    // update values by adding the input from the encoders to the current values
    auto updated = addEncoderDeltaToVA(channelValues.first, channelValues.second, inputStates.encoderState);

    // and write the updated values back to the hardware
    writeValuesToDACs(updated);

    // respond to the updates
    // outputBoard.writeStateToOutputBoard(outputStates);
}
/**
 * @brief Controller::syncWithUI Write all the required data to the UI
 */
void Controller::syncWithUI() {
    /**
     * Send uiBuffer through the FIFO to the ui
     */
    vector<IPCData> ipcData;
    //ipcData.push_back()
    //ipcHandler->sendAsync();
}

/**
 * @brief Controller::addEncoderDeltaToVA: add the number of pulses from the encoders to the prev. value read
 * @return the updated values computed
 */
pair<VAValues, VAValues> Controller::addEncoderDeltaToVA(VAValues A, VAValues B, EncoderState encoderState) const {

    // add the number of encoder pulses registered in the prev. cycle to the current values
    A.voltage += encoderState.encoderAV;
    A.current += encoderState.encoderAA;

    B.voltage += encoderState.encoderBV;
    B.current += encoderState.encoderBA;

    return {A, B};
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
