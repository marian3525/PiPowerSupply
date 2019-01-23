#include "Controller.hpp"

Controller::Controller()
{
    // setup the thread which handles the IPC
    ipcReceiverThread = std::thread(&Controller::ipcHandler, this);

    // start by showing the slash screen and then load the last used screen before shutting down

}

void Controller::ipcHandler(Controller* parent) {

    unsigned char* buffer = new unsigned char[100];
    int fd = 0;
    uint16_t totalSize = 0;
    uint16_t readSize = 0;
    bool quitting = false;

    while(!quitting) {
        while(readSize < totalSize)
            readSize += read(fd, buffer, totalSize);        // blocking call, waiting for data to arrive in the FIFO

        // process the received data
        parent->updateScreen(buffer);
    }
}

void Controller::updateScreen(unsigned char *data) {
    // depending on which screen is currently shown, do the appropritate update
    switch(currentScreen) {
    case(powerSupplyScreen): {
        break;
    }
    case(frequencyGenerator): {
        break;
    }
    case(programmableSupply): {
        break;
    }
    case(splash): {
        break;
    }
    case(dummyLoad): {
        break;
    }
    }
}

Controller::~Controller() {
     ipcReceiverThread.join();
}
