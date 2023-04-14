#include <map>
#include <iostream>
#include <conio.h>
#include "MenuFunctions.h"

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
    Exit
};

void InitializeMenuNameMap(std::map<std::string, MenuNames> &);

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
    _map["Exit"] = Exit;
}
