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
    // logging system initialization
    loggingSystem *log = new loggingSystem();
    // log->setDebug(true)E;

    // Instantiate the graph of the application
    GenericGraph *mainGraph = new GenericGraph();

    // menu tree initialization
    menu *mainMenu = menu::InstantiateMenu(log,mainGraph);

    // main loop
    mainMenu->GetMenuCMD();

    delete log;
    delete mainMenu;

    std::cout << "Press any key to close the console.";
    getch();
    return 0;
}