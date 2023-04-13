#include <iostream>
#include "utils.h"

class loggingSystem
{
private:
    bool clearLog;
    bool debug;

public:
#pragma region Constructors prototypes
    loggingSystem();
    loggingSystem(bool, bool);
    ~loggingSystem();
#pragma endregion
#pragma region Modifiers prototypes
    bool getDebug();
    void setDebug(bool);
    bool getClearConsole();
    void setClearConsole(bool);
#pragma endregion
#pragma region Functions prototypes
    void ClearConsole();
#pragma endregion
};

#pragma region Constructors definitions
loggingSystem::loggingSystem()
{
    // Setting debug to false for default use-case
    this->debug = false;

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
#pragma endregion
#pragma region Modifiers definitions
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
#pragma endregion
#pragma region Functions definitions
void loggingSystem::ClearConsole()
{
    // This clears the console
    system("CLS");
}
#pragma endregion
