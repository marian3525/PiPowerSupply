#pragma once
#include <vector>
#include <cinttypes>

enum class ButtonName {up, down, left, right, select, currentMode};

// from input board
struct EncoderState {
    uint8_t encoderAV;
    uint8_t encoderAA;
    uint8_t encoderBV;
    uint8_t encoderBA;
};


// from input board
struct InputStates {
    EncoderState encoderState;
    std::vector<std::pair<ButtonName, bool>> buttonStates;
    int8_t tempPSU;
    int8_t tempA;
    int8_t tempB;
};

class InputBoard
{

public:
    explicit InputBoard();
    InputStates readStateFromInputBoard();

};
