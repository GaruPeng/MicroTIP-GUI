#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H


class Multiplexer
{
public:
    Multiplexer();
    void setInput(char input);
    char getInput();
private :
    char input;
};

#endif // MULTIPLEXER_H