#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Controller* controller, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
