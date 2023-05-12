#include <iostream>
#include "utils.h"

class loggingSystem
{
private:
    bool clearLog = true;
    bool debug = false;

public:
    loggingSystem();
    loggingSystem(bool, bool);
    ~loggingSystem();
    
    bool getDebug();
    void setDebug(bool);
    bool getClearConsole();
    void setClearConsole(bool);
    void ClearConsole();
};

loggingSystem::loggingSystem()
{
    std::cout << "Hello!" << std::endl;

    const char *q = "Would you like to erase the console after each message? (Y/N)";
    this->clearLog = GetQuestionAnswer(q);
    if (clearLog)
        ClearConsole();
}
loggingSystem::loggingSystem(bool _clearLog, bool _debug)
{
    this->clearLog = _clearLog;
    this->debug = _debug;
}
loggingSystem::~loggingSystem()
{
    // Print a message only if debugging is set to true
    if (this->debug)
    {
        std::cout << "DEBUG: Logging system erased!" << std::endl;
    }
}

bool loggingSystem::getDebug()
{
    return this->debug;
}
void loggingSystem::setDebug(bool _debug)
{
    this->debug = _debug;
}
bool loggingSystem::getClearConsole()
{
    return this->clearLog;
}
void loggingSystem::setClearConsole(bool _clearConsole)
{
    this->clearLog = _clearConsole;
}

void loggingSystem::ClearConsole()
{
    // This clears the console
    system("CLS");
}
