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
    static int paddingOfString(int, int);
    static int paddingOfString(int);
    static std::string stringWithPadding(const char *, int, char, bool);
    static std::string stringWithPadding(const char *, int, char);
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

// This function takes as argument the length of the message it will add the padding to
int paddingOfString(int strLength, int ignoreCount)
{
    return ((lineLength - strLength) / 2 - ignoreCount);
}

int paddingOfString(int strLength)
{
    return paddingOfString(strLength, 0);
}

std::string stringWithPadding(const char *str, int strLength, char filler, bool isSelected)
{
    if (isSelected)
        return (std::string(paddingOfString(strLength + 6), filler) +
                ">> " +
                std::string(str) +
                " <<" +
                std::string(paddingOfString(strLength + 6), filler) +
                "\n");
    else
        return (std::string(paddingOfString(strLength), filler) +
                std::string(str) +
                std::string(paddingOfString(strLength), filler) +
                "\n");
}

std::string stringWithPadding(const char *str, int strLength, char filler)
{
    return stringWithPadding(str, strLength, filler, false);
}
