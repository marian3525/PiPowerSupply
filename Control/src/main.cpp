#include "controller/Controller.hpp"
#include "utils/IPCHandler.hpp"
#include <iostream>
using namespace std;

int main() {
//    Controller controller{};
//    controller.start();
    vector<uint8_t> data{0x00, 0x01,0x02, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    for(unsigned int i=15; i<30; i++) {
        data.push_back(i);
    }

    IPCHandler h{};
    IPCData d{};

    d.data = data.data();
    d.length = data.size();
    d.key = 1234;

    //h.sendPieceAsync(d);

    vector<uint8_t> data1{0x00, 0x01,0x02, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    for(unsigned int i=0; i<10; i++) {
        data1.push_back(i);
    }

    IPCData d1{};

    d1.data = data1.data();
    d1.length = data1.size();
    d1.key = 1234;

    auto v = vector<IPCData>();
    v.push_back(d);
    v.push_back(d1);

    h.sendPackAsync(v);

    return 0;
}
