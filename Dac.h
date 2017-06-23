#ifndef DAC_H
#define DAC_H

#include <stdint.h>

class Dac
{
public:
    Dac();
    uint16_t getValue();
    void setValue(uint16_t value);
private:
    uint16_t value;
};

#endif // DAC_H
