#include "Dac.h"

Dac::Dac() : value(0)
{

}



uint16_t Dac::getValue()
{
    return value;
}

uint8_t Dac::getCmdAdr()
{
    return cmdadr;
}

void Dac::setValue(uint16_t value)
{
    this->value = value;
}

void Dac::setCmdAdr(uint8_t cmdadr)
{
    this->cmdadr = cmdadr;
}
