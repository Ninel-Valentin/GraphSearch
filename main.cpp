#include <iostream>
#include <conio.h>
#include "headers/loggingSystem.h"
#include "headers/menu.h"

int main()
{
    loggingSystem *log = new loggingSystem();
    log->setDebug(false);

    menu *mainMenu = menu::InstantiateMenu(log);

    mainMenu->GetMenuCMD();

    std::cout << "Press any key to close the console.";
    getch();

    delete log;
    delete mainMenu;
    getch();
    return 0;
}