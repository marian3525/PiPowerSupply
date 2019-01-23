#ifndef FREQUENCYGENERATOR_HPP
#define FREQUENCYGENERATOR_HPP

#include <QMainWindow>

namespace Ui {
class FrequencyGenerator;
}

class FrequencyGenerator : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrequencyGenerator(QWidget *parent = nullptr);
    ~FrequencyGenerator();

private:
    Ui::FrequencyGenerator *ui;
};

#endif // FREQUENCYGENERATOR_HPP
