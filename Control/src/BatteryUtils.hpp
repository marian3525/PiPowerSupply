#ifndef BATTERYUTILS_HPP
#define BATTERYUTILS_HPP
/**
 * @brief The BatteryUtils class has the capacity to control outputs and sense inputs needed to charge/discharge a battery
 * It has 2 modes: charge (with other submodes) and discharge(with other submods)
 * The charge mode: a list of states. A state means {CC/CV of voltage V and current A until X condition (which can be time, charge or temperature)}
 * The discharge mode: a list of states
 */
class BatteryUtils
{
    enum BatteryMode{charge, discharge};
    enum Output{channelA, channelB};
public:
    BatteryUtils(Output outputChannel, BatteryMode mode, bool usesThermals);
    void executeTask();
    void cancelTask();
};

#endif // BATTERYUTILS_HPP
