#include "Controller.hpp"

Controller::Controller()
{
    // setup the thread which handles the IPC
    ipcReceiverThread = std::thread(&Controller::ipcHandler, this);

    // start by showing the splash screen and then load the last used screen before shutting down

}

void Controller::ipcHandler(Controller* parent) {

    unsigned char* readBuffer = new unsigned char[100];
    unsigned char* writeBuffer = new unsigned char[100];

    int fdRead = open("../Control/src/controllerToUI", O_RDONLY);
    int fdWrite = open("../Control/src/controllerToUI", O_WRONLY);

    const uint16_t totalRead = 1;
    uint16_t readSize = 0;

    const uint16_t totalWrite = 1;
    uint16_t writtenSize = 0;

    bool quitting = false;

    while(!quitting) {
        while(readSize < totalRead) {
            readSize += read(fdRead, readBuffer, totalRead);        // blocking call, waiting for data to arrive in the FIFO
        }

        while(writtenSize < totalWrite) {
            writtenSize += write(fdWrite, writeBuffer, totalWrite);
        }

        // process the received data
        parent->updateScreen(readBuffer, totalRead);
    }
}

void Controller::updateScreen(unsigned char *data, uint16_t totalSize) {
    // depending on which screen is currently shown, do the appropritate update

    for(int i=0; i<totalSize; i++) {
        printf("%x", data[i]);
    }

    return;

    /*
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
    */
}

Controller::~Controller() {
     ipcReceiverThread.join();
}
