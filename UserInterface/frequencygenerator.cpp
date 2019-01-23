#include "FrequencyGenerator.h"
#include "ui_FrequencyGenerator.h"

FrequencyGenerator::FrequencyGenerator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FrequencyGenerator)
{
    ui->setupUi(this);
}

FrequencyGenerator::~FrequencyGenerator()
{
    delete ui;
}
