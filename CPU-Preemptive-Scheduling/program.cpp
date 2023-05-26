#include "program.h"

program::program(std::string name_, int PC_, int pid_)
{
    name = name_;
    startPC = PC_;
    PC = PC_;
    ACC = 0;
    pid = pid_;
    finished = false;
    blocked = false;
}
