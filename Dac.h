#ifndef DAC_H
#define DAC_H

#include <stdint.h>

class Dac
{
public:
    Dac();
    uint16_t getValue();
    uint8_t getCmdAdr();
    void setCmdAdr(uint8_t cmdadr);
    void setValue(uint16_t value);
private:
    uint16_t value;
    uint8_t cmdadr;
};

typedef enum
{
    DAC_WRITE_UPDATE_CH1    = 0x18,
    DAC_WRITE_UPDATE_CH2    = 0x19,
    DAC_WRITE_UPDATE_CH3    = 0x1A,
    DAC_WRITE_UPDATE_CH4    = 0x1B,
    DAC_WRITE_UPDATE_CHall  = 0x1F

}DACreg;

#endif // DAC_H
