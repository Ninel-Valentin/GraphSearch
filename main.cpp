#include <iostream>
#include <conio.h>

#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H
#include "headers/loggingSystem.h"
#endif

#ifndef MENU_H
#define MENU_H
#include "headers/Menu/menu.h"
#endif

int main()
{
    bool passed = false;
    do
        if (CheckForPassword())
        {
            std::cout << "Correct password! Welcome!"
                      << std::endl
                      << "Press any key to continue..."
                      << std::endl;
            getch();
            passed = true;
            break;
        }
    while (GetQuestionAnswer("Wrong password provided... Want to try again? (Y/N)"));
    if (passed)
    {
        // logging system initialization
        loggingSystem *log = new loggingSystem();
        // log->setDebug(true);

        // Instantiate the graph of the application
        GenericGraph *mainGraph = new GenericGraph();

        // menu tree initialization
        menu *mainMenu = menu::InstantiateMenu(log, mainGraph);

        // main loop
        mainMenu->GetMenuCMD();

        delete log;
        delete mainMenu;
    }

    std::cout << "Press any key to close the console.";
    getch();
    return 0;
}