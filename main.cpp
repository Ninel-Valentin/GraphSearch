#include <iostream>
#include <conio.h>
#include "headers/loggingSystem.h"
#include "headers/menu.h"

int main()
{
    // logging system initialization
    loggingSystem *log = new loggingSystem();

    // menu tree initialization
    menu *mainMenu = menu::InstantiateMenu(log);

    // main loop
    mainMenu->GetMenuCMD();

    delete log;
    delete mainMenu;
    std::cout << "Press any key to close the console.";
    getch();
    return 0;
}