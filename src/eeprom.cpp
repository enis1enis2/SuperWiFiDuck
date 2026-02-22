#include "eeprom0.h"

#include "config.h"

namespace eeprom {
    void begin() {
        EEPROM.begin(EEPROM_SIZE);
    }
};