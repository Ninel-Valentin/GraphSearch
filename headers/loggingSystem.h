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

    // Get the debug value
    bool getDebug();
    // Toggle the debug logs
    void setDebug(bool);
    // Get the clear console value
    bool getClearConsole();
    // Toggle the clear console after each message
    void setClearConsole(bool);
    void ClearConsole();
    // This function takes as argument the length of the message it will add the padding to and the length of ignored characters into the padding
    static int paddingOfString(int, int);
    // This function takes as argument the length of the message it will add the padding to
    static int paddingOfString(int);
    // This function logs in the console the str array with a filler arround to match a certain size, can be selected to also show the name between arrows ">> str <<"
    static void stringWithPadding(const char *, int, char, bool);
    // This function logs in the console the str array with a filler arround to match a certain size
    static void stringWithPadding(const char *, int, char);
};

loggingSystem::loggingSystem()
{
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

int paddingOfString(int strLength, int ignoreCount)
{
    return ((lineLength - strLength) / 2 - ignoreCount);
}

int paddingOfString(int strLength)
{
    return paddingOfString(strLength, 0);
}

void ShowStringWithPadding(const char *str, int strLen, char filler, bool isSelected)
{
    int repeatCount;
    if (isSelected)
        // + 6 because when selected there are the arrows on both side ">> " & " <<" -> 3 chars on each side
        repeatCount = paddingOfString(strLen + 6);
    else
        repeatCount = paddingOfString(strLen);

    for (int i = 0; i < repeatCount; i++)
        std::cout << filler;
    if (isSelected)
        std::cout << ">> " << str << " <<";
    else
        std::cout << str;
    for (int i = 0; i < repeatCount; i++)
        std::cout << filler;
    std::cout << std::endl;
}

void ShowStringWithPadding(const char *str, int strLen, char filler)
{
    ShowStringWithPadding(str, strLen, filler, false);
}
