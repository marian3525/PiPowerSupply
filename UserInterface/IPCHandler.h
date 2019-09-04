#ifndef IPCHANDLER_H
#define IPCHANDLER_H
#include <thread>
#include <cinttypes>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <future>
#include <fcntl.h>
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

class IPCHandler
{
public:
    IPCHandler();
    void readPieceAsync(std::function<void(IPCData)> callback);
    void readPackAsync(std::function<void(std::vector<IPCData>)>);

private:
    int readDescriptor;
    std::thread ipcWorkerThread;
    //Controller* controller;

    std::mutex mtx;
    void onRead(IPCData data);
    static void readAll(int fd, void* data, size_t size);
};

#endif // IPCHANDLER_H
