#include <QMainWindow>

namespace Ui {
class PowerSupplyScreen;
}

class PowerSupplyScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit PowerSupplyScreen(QWidget *parent = nullptr);
    ~PowerSupplyScreen();

private:
    Ui::PowerSupplyScreen *ui;
};
