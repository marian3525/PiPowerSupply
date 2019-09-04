#include <fcntl.h>
#include <cerrno>
#include <future>
#include <iostream>
#include <cstring>
#include "IPCHandler.hpp"
#include "constants.hpp"

IPCHandler::IPCHandler() {
    printf("clearing the fifo file\n");
    remove(constants::FIFO_NAME);

    printf("creating pipe\n");
    // create a pipe to send data to the UI
    int status = mkfifo(constants::FIFO_NAME, 0666);

    if(status == -1) {
        printf("Error on mkfifo(). Error %d: %s\n", errno, strerror(errno));
    }

    printf("opening pipe\n");
    // blocks until the other process opens the pipe for reading
    writeDescriptor = open(constants::FIFO_NAME, O_WRONLY);

    if(writeDescriptor == -1) {
        std::cout<<"Failed to open pipe. ERRNO: "<<errno<<": "<<strerror(errno)<<std::endl;
    }
}

/**
 *
 * @param data: IPC data to be sent, containing the key, data length and actual data bytes
 */
void IPCHandler::sendPieceAsync(IPCData data) {

    auto f = std::async(std::launch::async, [&] {
                            mtx.lock();

                            writeAll(writeDescriptor, &data.key, sizeof(data.key));
                            writeAll(writeDescriptor, &data.length, sizeof(data.length));
                            writeAll(writeDescriptor, data.data, data.length);

                            mtx.unlock();
                        }
        );
}

void IPCHandler::sendPackAsync(std::vector<IPCData> pack) {
    auto f = std::async(std::launch::async, [&] {
                   IPCData packInitData{};
                   packInitData.key = 0;
                   packInitData.length = pack.size();   // only this length is used to determine the length of the
                                                        // pack (no. of IPCData in the vector)
                   packInitData.data = new uint8_t[packInitData.length];  // dummy data

                   // send the length of the pack first so that the receiver knows how much to expect

                   mtx.lock();

                   writeAll(writeDescriptor, &packInitData.key, sizeof(packInitData.key));
                   writeAll(writeDescriptor, &packInitData.length, sizeof(packInitData.length));
                   writeAll(writeDescriptor, packInitData.data, packInitData.length);

                   for (auto data : pack) {
                       writeAll(writeDescriptor, &data.key, sizeof(data.key));
                       writeAll(writeDescriptor, &data.length, sizeof(data.length));
                       writeAll(writeDescriptor, data.data, data.length * sizeof(uint8_t));
                   }

                   mtx.unlock();

               }
        );

}

IPCHandler::~IPCHandler() {
    printf("Deleting pipe file");
    remove(constants::FIFO_NAME);
}

void IPCHandler::writeAll(int fd, const void* data, size_t size) {
    uint32_t totalWritten = 0;
    while(totalWritten < size) {
        totalWritten += write(fd, (unsigned char*)data + totalWritten, size-totalWritten);
    }
}

