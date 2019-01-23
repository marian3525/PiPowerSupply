#include "InputBoard.hpp"
#include <cinttypes>
#include "wiringPiSPI.h"

InputBoard::InputBoard()
{

}

/**
 * @brief Controller::getStateFromInputBoard: init SPI comms with the input board, receive the bytes and build the struct
 * Data layout in the buffer:]
 * 0: encoderAV
 * 1: encoderAA
 * 2: encoderBV
 * 3: encoderBA
 * 4: tempPSU
 * 5: tempA
 * 6: tempB
 * 7: button_up
 * 8: button_down
 * 9: button_left
 * 10: button_right
 * 11: button_select
 * 12: button_currentMode
 * @return InputStates containing the read data
 */

InputStates InputBoard::readStateFromInputBoard() {
    const uint8_t bufferSize = 12;
    auto buffer = new uint8_t[bufferSize];
#if RUNNING_ON_RASPBIAN == 1
    wiringPiSPIDataRW(0, buffer, bufferSize);
#endif

    // extract the data
    uint8_t encAV, encAA, encBV, encBA, up, down, left, right, select, currentMode;
    int8_t tempPSU=0, tempA=0, tempB=0;

    encAV = buffer[0];
    encAA = buffer[1];
    encBV = buffer[2];
    encBA = buffer[3];

    // use OR to transfer bits from unsigned to signed.
    //The value sent from the board is actually a signed one, but packed into unsigned for transmission.
    tempPSU |= buffer[4];
    tempA |= buffer[5];
    tempB |= buffer[6];

    up = buffer[7];
    down = buffer[8];
    left = buffer[9];
    right = buffer[10];
    select = buffer[11];
    currentMode = buffer[12];

    delete[] buffer;
    return InputStates{{encAV, encAA, encBV, encBA},
                       {{ButtonName::up, up}, {ButtonName::down, down}, {ButtonName::left, left},
                        {ButtonName::right, right}, {ButtonName::select, select},
                        {ButtonName::currentMode, currentMode}},
                        tempPSU, tempA, tempB};
}
