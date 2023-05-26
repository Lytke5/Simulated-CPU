#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <queue>
#include "program.h"
#define MAX_STACK 256
#define USE_PREEMPTIVE_SCHEDULING false

int readProgram(std::string *IM, std::string fileName, int* maxIM)
{
    std::ifstream file;
    std::string line;
    int newPC = *maxIM;
    file.open(fileName);

    if(file.is_open())
    {
        IM += *maxIM;

        while(std::getline(file, line))
        {
            *IM = line;
            IM++;
            (*maxIM)++;
        }
    }
    file.close();

    return newPC;
}
void LOAD(int value, int* ACC)
{
    *ACC = value;
}

void ADD(int value, int* ACC)
{
    *ACC += value;
}

void SUB(int value, int* ACC)
{
    *ACC -= value;
}

void PERIPHERAL(program* p, int* EA)
{
    *EA = 4;
    p->blocked = true;
}

int EXECUTE(std::string* IM, std::string value, int *maxIM, program* p)
{
    p->blocked = true;
    return readProgram(IM, value, maxIM);
}

int EXIT()
{
    return -1;
}

int readOperation(std::string* IM, std::string command, int* ACC, int* maxIM, program* p, int* EA)
{
    char operation = command[0];
    std::string value;
    if(command.size() > 2)
        value = command.substr (2);


    if(operation == 'L')
        LOAD(std::stoi(value), ACC);
    else if(operation == 'A')
        ADD(std::stoi(value), ACC);
    else if(operation == 'S')
        SUB(std::stoi(value), ACC);
    else if(operation == 'P')
        PERIPHERAL(p,EA);
    else if(operation == 'X')
        return EXECUTE(IM, value, maxIM, p);
    else if(operation == 'Z')
        return EXIT();


    return 0;
}

int main()
{
    int maxQuantum = 5;
    std::string instructionMemory[MAX_STACK];
    int maxIM = 0;
    int EA = 0;
    bool EAfinished;
    int ACC = 0;
    int PC = 0;
    int highestPid = 0;
    std::queue <program*> programList;
    std::vector <program*> finishedprograms;

    program *init = new program("init", 0, 0);
    programList.push(init);



    readProgram(&instructionMemory[0], "init", &maxIM);

    std::cout << "Takt PID  Prozess   PC        ACC       E/A   Befehl" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
    int takt = 0;

    while(true){

        program* currProgram;
        if(programList.size() != 0) //hole nächsten Prozess
        {
            currProgram = programList.front();
            programList.pop();
        }
        else
        {
            break;
        }


        if(currProgram->blocked == true && EA > 0) //Kein Prozess ist aktuell aktiv
        {
            if(EA > 0) //ernidrige EA falls an
            {
                EA--;
                bool isActivated = false;
                if(EA == 0)
                {
                    EAfinished = true;
                }
            }
            std::cout << std::left << std::setw(5) << takt;
            takt++;
            std::cout << std::left << std::setw(5) << currProgram->pid;
            std::cout << std::left << std::setw(10) << "WAITING";
            std::cout << std::left << std::setw(10) << "-";
            std::cout << std::left << std::setw(10) << "-";
            std::cout << std::left << std::setw(10) << EA;
            std::cout << std::left << std::setw(10) << "-";
            std::cout << std::endl;
        }

        if(EAfinished && currProgram->blocked == true) //EA ist fertig und ein Prozess soll aufgeweckt werden
        {
            currProgram->blocked = false;
            EAfinished = false;
        }

        while(currProgram->finished == false && currProgram->blocked == false)
        {
            PC = currProgram->PC;
            ACC = currProgram->ACC;
            if(currProgram->PC - currProgram->startPC >= maxQuantum && USE_PREEMPTIVE_SCHEDULING)
            {
                currProgram->blocked = true;
                break;
            }
            if(EA > 0)
            {
                EA--;
                bool isActivated = false;
                if(EA == 0)
                {
                    EAfinished = true;
                }
            }
            std::cout << std::left << std::setw(5) << takt;
            takt++;
            std::cout << std::left << std::setw(5) << currProgram->pid;
            std::cout << std::left << std::setw(10) <<currProgram->name;

            int newPC = readOperation(&instructionMemory[0], instructionMemory[PC], &ACC, &maxIM, currProgram, &EA);
            if(newPC == -1) //Prozess beenden
            {
                currProgram->ACC = ACC;
                currProgram->finished = true;
                finishedprograms.push_back(currProgram);
            }
            else if(newPC != 0) //neuen Prozess erstellen
            {
                currProgram->blocked = true;
                std::string name = instructionMemory[PC].substr(2);
                program* newProgram = new program(name, newPC, ++highestPid);

                std::vector <program*> temps;
                for(int k = 0; k < programList.size(); k++)
                {
                    temps.push_back(programList.front());
                    programList.pop();
                }
                programList.push(newProgram);
                for(int k = 0; k < temps.size(); k++)
                {
                    programList.push(temps[k]);
                }
            }

            std::cout << std::left << std::setw(10) << PC;
            std::cout << std::left << std::setw(10) << ACC;
            std::cout << std::left << std::setw(10) << EA;
            std::cout << std::left << std::setw(10) << instructionMemory[PC];
            std::cout << std::endl;

            PC++;
            if(currProgram != nullptr)
            {
                currProgram->PC = PC;
                currProgram->ACC = ACC;
            }
        }
        if(currProgram->finished == false)
        {
            programList.push(currProgram);

        }
        if(EA == 0)
        {
            currProgram->blocked = false;
        }
    }

    std::cout << std::endl << std::endl;
    std::cout << "Zusammenfassung:" << std::endl;
    std::cout << "Pid  Prozess  Start     Ende   Verweilzeit    ACC" << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;

    for(int i = 0; i < finishedprograms.size(); i++)
    {
        std::cout << std::left << std::setw(5) << finishedprograms[i]->pid;
        std::cout << std::left << std::setw(10) <<finishedprograms[i]->name;
        std::cout << std::left << std::setw(10) << finishedprograms[i]->startPC;
        std::cout << std::left << std::setw(10) << finishedprograms[i]->PC;
        std::cout << std::left << std::setw(10) << finishedprograms[i]->PC - finishedprograms[i]->startPC;
        std::cout << std::left << std::setw(10) << finishedprograms[i]->ACC;
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "simulierte Takte: " << takt << std::endl;
}

