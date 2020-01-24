#include "Multiplexer.h"

Multiplexer::Multiplexer() : input(0)
{

}

void Multiplexer::setInput(char input)
{
    if((input>0) && (input<5))
    {
        this->input = input;
    }
    else
    {
        this->input = 0;
    }
}

char Multiplexer::getInput()
{
    return this->input;
}
