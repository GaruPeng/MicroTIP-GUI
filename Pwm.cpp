#include "Pwm.h"

PWM::PWM():freq(0),dutycycle(0)
{

}

uint16_t PWM::getFreq()
{
    return freq;
}

void PWM::setFreq(uint16_t value)
{
    this->freq = value;
}


uint8_t PWM::getDuty()
{
    return dutycycle;
}

void PWM::setDuty(uint8_t value)
{
    this->dutycycle = value;
}
