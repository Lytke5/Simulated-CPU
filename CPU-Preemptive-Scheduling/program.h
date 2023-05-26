#ifndef PROGRAM_H
#define PROGRAM_H
#include <iostream>

class program
{
public:
    std::string name;
    int ACC;
    int PC;
    int startPC;
    int pid;
    bool blocked;
    bool finished;
    program(std::string name_, int PC_, int pid_);
};

#endif // PROGRAM_H
