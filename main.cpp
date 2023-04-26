#include <iostream>
#include <conio.h>
#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H
#include "headers/loggingSystem.h"
#endif
#include "headers/menu.h"
#include "headers/Graphs/GenericGraphs.h"

int main()
{
    // logging system initialization
    loggingSystem *log = new loggingSystem();

    // // menu tree initialization
    // menu *mainMenu = menu::InstantiateMenu(log);

    // // main loop
    // mainMenu->GetMenuCMD();

    GenericGraph *graph = new GenericGraph(log);
    graph->InitializeGraph();

    graph->Solve();

    delete log;
    // delete mainMenu;
    std::cout << "Press any key to close the console.";
    getch();
    return 0;
}