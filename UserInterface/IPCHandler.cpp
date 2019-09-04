#include "IPCHandler.h"
#include "../Control/src/utils/constants.hpp"
#include <iostream>
#include <cstring>

IPCHandler::IPCHandler()
{
    printf("opening pipe\n");
    readDescriptor = open(constants::FIFO_NAME, O_RDONLY);

    if(readDescriptor == -1) {
        std::cout<<"Failed to open pipe. ERRNO: "<<errno<<": "<<strerror(errno)<<std::endl;
    }

    if(errno == 2) {
        printf("FIFO not opened for write, exiting\n");
        exit(-1);
    }
}

void IPCHandler::readPackAsync(std::function<void (std::vector<IPCData>)> callback) {
    // start sending the data to the ui and free the resources used in that thread afterwards
    auto f = std::async(std::launch::async, [&](){
        uint32_t key;
        uint32_t vectorLength;
        uint32_t dataLength;
        uint8_t* dataBuffer;
        std::vector<IPCData> dataList;

        // read the initial pack containing the length of the list to be read
        mtx.lock();
        // read the key
        readAll(readDescriptor, &key, sizeof(key));

        // read the data length
        readAll(readDescriptor, &vectorLength, sizeof(vectorLength));

        dataBuffer = new uint8_t[vectorLength];
        // read the data
        readAll(readDescriptor, dataBuffer, vectorLength * sizeof(uint8_t));

        // delete the unused pointer
        delete dataBuffer;

        for(unsigned int i=0; i < vectorLength; i++) {
            // read the key
            readAll(readDescriptor, &key, sizeof(key));

            // read the data length
            readAll(readDescriptor, &dataLength, sizeof(dataLength));

            dataBuffer = new uint8_t[dataLength];
            // read the data
            readAll(readDescriptor, dataBuffer, dataLength * sizeof(uint8_t));

            IPCData d;

            d.key = key;
            d.length = dataLength;
            d.data = dataBuffer;

            dataList.push_back(d);

            // infinite loop here
        }

        mtx.unlock();

        // when complete, call the onRead method and provide the IPCData struct
        callback(dataList);
    }
    );
}

void IPCHandler::readPieceAsync(std::function<void(IPCData)> callback) {

    // start sending the data to the ui and free the resources used in that thread afterwards
    auto f = std::async(std::launch::async, [&](){
        IPCData data;
        uint32_t key;
        uint32_t dataLength;
        uint8_t* dataBuffer;

        mtx.lock();
        // read the key
        printf("reading key\n");
        readAll(readDescriptor, &key, sizeof(uint32_t));

        printf("reading length\n");
        // read the data length
        readAll(readDescriptor, &dataLength, sizeof(uint32_t));

        dataBuffer = new uint8_t[dataLength];
        printf("reading data\n");
        // read the data
        readAll(readDescriptor, dataBuffer, dataLength);

        mtx.unlock();

        // init the struct
        data.key = key;
        data.length = dataLength;
        data.data = dataBuffer;

        // when complete, call the onRead method and provide the IPCData struct
        callback(data);

    }
    );
}

void IPCHandler::readAll(int fd, void* data, size_t size) {
    uint32_t totalRead = 0;

    while(totalRead < size) {
        totalRead += read(fd, (unsigned char*)data + totalRead, size-totalRead);
    }
}
