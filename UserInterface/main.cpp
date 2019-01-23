#include <QApplication>
#include "Controller.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller controller{};
    return a.exec();
}
