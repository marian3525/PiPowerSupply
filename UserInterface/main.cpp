#include <QApplication>
#include "MainWindow.hpp"
#include "IPCHandler.h"

/**
 * @brief onRead Called after data has been read from the pipe
 * @param data Data that was readl
 */
void onReadV(std::vector<IPCData> data) {
    printf("%d", (int) data.size());
}

void onRead(IPCData data) {
    printf("%d", data.length);
}


int main()
{
//    Controller controller{};
//    QApplication a{};
//    QMainWindow* window = new MainWindow{ &controller };
//    window->show();

//    return a.exec();

    printf("started\n");
    IPCHandler h{};

    h.readPackAsync(onReadV);

    printf("\nended");
    return 0;
}
