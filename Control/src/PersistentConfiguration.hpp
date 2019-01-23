#pragma once

/**
 * @brief The PersistentConfiguration class stores values that should be stored between program runs
 */

class PersistentConfiguration
{
public:
    PersistentConfiguration();
    void store() const;
    void read() const;
};
