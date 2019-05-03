#pragma once

#include <vector>
#include <cinttypes>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "Converters.hpp"
#include "InputBoard.hpp"
#include "OutputBoard.hpp"

using namespace std;

// tmp store from/to ADC/DAC
struct VAValues {
    uint16_t voltage;
    uint16_t current;
};

// tmp store to send to the UI via main
struct ChannelState {
    VAValues VAvalues;
    int8_t temperature;
};


class Controller
{
public:
    explicit Controller();
    ~Controller();
    void start();
    // read only functions: of interest to the UI
    pair<ChannelState, ChannelState> getChannelsState() const;
    vector<pair<ButtonName, bool>> getButtonStates() const;

private:
    // update method: get and send data to the connected devices, syncing the buffers with the hardware
    void syncWithHardware();
    // send data to the UI using the IPC thread
    void syncWithUI();

    void IOinit();
    void SPIinit();

    pair<VAValues, VAValues> addEncoderDeltaToVA() const;
    EncoderState getEncoderDeltas() const;

    // misc. functionality, battery charger/discharger mode, programmable output (vs time)

    Converters converter{};
    InputBoard inputBoard{};
    OutputBoard outputBoard{};

    //comms methods
    pair<ChannelState, ChannelState> readChannelStatesFromADCs();
    void writeStatesToDACs(pair<ChannelState, ChannelState> outState);

    // buffers to store the data received from/to be sent to the periperals
    OutputStates outputStates;
    vector<pair<ButtonName, bool>> buttonStates;
    InputStates inputStates;
    pair<ChannelState, ChannelState> channelStates;
    EncoderState encoderState{0, 0, 0, 0};

    // IPC comms
    // file descriptor for the FIFO
    int descriptorR;
    const uint16_t uiReceiveBufferSize = 100;
    uint8_t* uiReceiveBuffer = new uint8_t[uiReceiveBufferSize];

    int descriptorW;
    const uint16_t uiSendBufferSize = 100;
    uint8_t* uiSendBuffer = new uint8_t[uiSendBufferSize];
};
