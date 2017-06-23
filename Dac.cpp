#include "Dac.h"

Dac::Dac() : value(0)
{

}

uint16_t Dac::getValue()
{
    return value;
}

void Dac::setValue(uint16_t value)
{
    this->value = value;
}
