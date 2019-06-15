#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    Controller controller{};
    QApplication a{ argc, argv };
    QMainWindow* window = new MainWindow{ &controller };
    window->show();

    return a.exec();
}
