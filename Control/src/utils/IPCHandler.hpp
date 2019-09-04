//
// Created by marian on 02.09.2019.
//

#ifndef PSUCONTROLLER_IPCHANDLER_HPP
#define PSUCONTROLLER_IPCHANDLER_HPP
#include <cinttypes>
#include <sys/types.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <vector>

struct IPCData {
    // key of the updated value
    uint32_t key;

    // length of the data
    uint32_t length;

    // actual data to be sent
    uint8_t* data;
};

class IPCHandler {

public:
    IPCHandler();
    void sendPieceAsync(IPCData data);
    void sendPackAsync(std::vector<IPCData> pack);
    static void writeAll(int fd, const void* data, size_t size);
    ~IPCHandler();

private:
    int writeDescriptor;
    std::thread ipcWorkerThread;
    std::mutex mtx;
};


#endif //PSUCONTROLLER_IPCHANDLER_HPP
