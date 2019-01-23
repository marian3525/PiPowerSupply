#include "SplashScreen.h"
#include "ui_SplashScreen.h"

SplashScreen::SplashScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
}

SplashScreen::~SplashScreen()
{
    delete ui;
}
