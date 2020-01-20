#ifndef PWM_H
#define PWM_H

#include <stdint.h>

class PWM
{
public:
    PWM();
    uint16_t getFreq();
    uint8_t getDuty();
    void setFreq(uint16_t value);
    void setDuty(uint8_t value);
private:
    uint16_t freq;
    uint8_t dutycycle;
};

#endif // PWM_H
