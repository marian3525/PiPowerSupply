#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>

class Controller
{
public:
    Controller();
    ~Controller();
private:
    void updateScreen(unsigned char* data, uint16_t totalSize);

    static void ipcHandler(Controller* parent);

    std::thread ipcReceiverThread;
    enum Screen { splash, powerSupplyScreen, frequencyGenerator, dummyLoad, programmableSupply };
    Screen lastScreen;
    Screen currentScreen;
};
