#include "powersupplyscreen.hpp"
#include "ui_powersupplyscreen.h"

PowerSupplyScreen::PowerSupplyScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PowerSupplyScreen)
{
    ui->setupUi(this);
}

PowerSupplyScreen::~PowerSupplyScreen()
{
    delete ui;
}
