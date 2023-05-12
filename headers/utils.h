#include <map>
#include <iostream>
#include <conio.h>
#include "Menu/MenuFunctions.h"

// Max line char for logging
const int lineLength = 50;

enum MenuNames
{
    Start_algorithm,
    Read_data,
    Display_data,
    Add_node,
    Delete_node,
    Add_link,
    Delete_link,
    Erase_data,
    Custom_location,
    Default_location,
    Toggle_Debug,
    Toggle_console_clear,
    Help,
    Exit
};

bool GetQuestionAnswer(const char *question)
{
    // Show the question
    std::cout << question << std::endl;

    // Repeat this step as long as the function does not return a value;
    while (true)
    {
        char confirmation = getch();
        std::cout << std::endl
                  << std::flush;

        if (confirmation == 'Y' || confirmation == 'y')
            return true;
        else if (confirmation == 'N' || confirmation == 'n')
            return false;
        else
            std::cout << "Please enter a valid answer for the question..."
                      << std::endl
                      << question
                      << std::endl;
    }
}

void InitializeMenuNameMap(std::map<std::string, MenuNames> &_map)
{
    _map["Start algorithm"] = Start_algorithm;
    _map["Read data"] = Read_data;
    _map["Display data"] = Display_data;
    _map["Add node"] = Add_node;
    _map["Delete node"] = Delete_node;
    _map["Add link"] = Add_link;
    _map["Delete link"] = Delete_link;
    _map["Erase data"] = Erase_data;
    _map["Custom location"] = Custom_location;
    _map["Default location"] = Default_location;
    _map["Toggle Debug"] = Toggle_Debug;
    _map["Toggle console clear"] = Toggle_console_clear;
    _map["Help"] = Help;
    _map["Exit"] = Exit;
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
