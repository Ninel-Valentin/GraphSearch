#include <iostream>
#include <fstream>

#ifndef ARRAY_H
#define ARRAY_H
#include "../Graphs/Array.h"
#endif

#ifndef GENERICGRAPHS_H
#define GENERICGRAPHS_H
#include "../Graphs/GenericGraphs.h"
#endif

class menu
{
private:
    int functionIndex;
    // If this has the value -1, it means we are not in selective mode, otherwise, it represents the index of the children
    int selectedChild = -1;
    char *name;
    int nameLength = 0;
    // Size initialized with 0 for menu entries which have no children
    int childrenSize = 0;
    menu **children;
    menu *parent;
    // This represents a reference to the logging system of the program
    loggingSystem *log;
    GenericGraph *graphData;

    static const char *dataPath;

public:
    menu();
    menu(int, char *, int, loggingSystem *, GenericGraph *);
    ~menu();

    static menu *InstantiateMenu(loggingSystem *, GenericGraph *);

    void PutMenu();
    void GetMenuCMD();
    void ExecuteFunction();
};
const char *menu::dataPath = "storage/menuSet.txt";

menu::menu()
{
}
menu::menu(int _nameLength, char *_name, int _childrenSize, loggingSystem *_log, GenericGraph *_graphData)
{
    delete[] this->name;
    this->nameLength = _nameLength;
    this->name = new char[this->nameLength + 1];

    // Copy the name of the menu
    for (int i = 0; i < this->nameLength; i++)
        this->name[i] = _name[i];

    this->childrenSize = _childrenSize;
    this->children = new menu *[_childrenSize];

    // // Instantiate empty menus
    // for (int i = 0; i < this->childrenSize; i++)
    //     this->children[i] = new menu();

    this->parent = new menu();

    // Add a reference to the logging system of the program
    this->log = _log;
    this->graphData = _graphData;
}
menu::~menu()
{
    if (nameLength > 0)
        delete[] name;
    if (childrenSize > 0)
        delete[] children;
}

menu *menu::InstantiateMenu(loggingSystem *_log, GenericGraph *_graph)
{
    // Declare the input file stream and open the file.
    std::ifstream MenuSetFile;
    MenuSetFile.open(dataPath);

    // Instantiate a int to know the size of Menus in the file
    int size;
    // Read the first line as it contains the number of menu entries in the file
    MenuSetFile >> size;

    // Read the length of the array from the first row from the file
    menu *menuObj = new menu[size];

    // We use this property as a temporary cursor for the children linking and reset it to -1 at the end
    for (int i = 0; i < size; i++)
        menuObj[i].selectedChild = 0;

    if (_log->getDebug())
        std::cout << "DEBUG: Reading menus from the file..."
                  << std::endl;

    // Initialize the data reader for the file
    // Each menu row is built as such "Function_Id Child_Count Parent_Id Name_Length Name_String"
    int reader,
        // Instantiate a menu counter
        menuIndex = 0;
    // Check if the file is open, otherwise show a message and stop the program
    if (MenuSetFile.is_open())
    {
        // Iterate the file data until we find the end of file OR until we read the specified number of menus
        while (!MenuSetFile.eof() && menuIndex != size)
        {

            // Read the children count of the Menu in the reader;
            MenuSetFile >> reader;
            // If the menu has children, modify the size and allocate memory for the children pointer array
            if (reader != -1)
                menuObj[menuIndex].functionIndex = reader;

            // Read the children count of the Menu in the reader;
            MenuSetFile >> reader;
            // If the menu has children, modify the size and allocate memory for the children pointer array
            if (reader != 0)
            {
                menuObj[menuIndex].childrenSize = reader;
                menuObj[menuIndex].children = new menu *[reader];
            }

            // Read the parent of the Menu in the reader;
            MenuSetFile >> reader;
            // -1 means no parent I.E. main menu, and for it we set the parent as nullptr
            if (reader == -1)
                menuObj[menuIndex].parent = nullptr;
            else
            {
                menuObj[menuIndex].parent = &menuObj[reader];
                int crrIndex = menuObj[reader].selectedChild;
                menuObj[reader].children[crrIndex] = &menuObj[menuIndex];
                menuObj[reader].selectedChild++;
            }

            // Read the menu name length of the Menu in the reader;
            MenuSetFile >> reader;
            // Initialize the name array and read it.
            menuObj[menuIndex].nameLength = reader;
            menuObj[menuIndex].name = new char[reader + 1];

            // Ignore a white space between the last number and the name
            MenuSetFile.ignore(256, ' ');
            MenuSetFile.getline(menuObj[menuIndex].name, reader + 1);

            // Add the static data
            menuObj[menuIndex].log = _log;
            menuObj[menuIndex].graphData = _graph;

            // Increment the counter
            menuIndex++;
        }
    }
    else
    {
        // If the file was not found, close the program
        std::cout << "Menu set file not found! Stopping process..."
                  << std::endl;
        getch();
        exit(1);
    }
    // Close the file
    MenuSetFile.close();

    // We reset the temporary property back to it's default value
    for (int i = 0; i < size; i++)
        menuObj[i].selectedChild = -1;

    // return the main object
    return menuObj;
}

void menu::PutMenu()
{
    // Check if the loggingSystem requires the console to be cleared
    if (this->log->getClearConsole())
        this->log->ClearConsole();

    ShowStringWithPadding("(PRESS [x] TO SWITCH MENU TYPE)", 31, ' ');

    // Calculate the padding required for each name/message to center them
    ShowStringWithPadding(this->name, this->nameLength, '-');

    for (int i = 0; i < this->childrenSize; i++)
    {
        std::cout << i
                  << ": ";
        ShowStringWithPadding(this->children[i]->name,
                              // the length of the string + the length of "i: " * 2 (for each side's padding)
                              this->children[i]->nameLength + 2 * (2 + (i < 10 ? 1 : 2)),
                              ' ',
                              this->selectedChild == i);
    }

    // Back command for nested menus
    if (this->parent)
    {
        std::cout << this->childrenSize
                  << ": ";
        ShowStringWithPadding("Back",
                              // the length of the string + (the length of ": " (2) + the length of i) * 2 (for each side's padding),
                              4 + (2 + (this->childrenSize < 10 ? 1 : 2)) * 2,
                              ' ',
                              this->selectedChild == this->childrenSize);
    }

    ShowStringWithPadding(this->name, this->nameLength, '-');
    std::cout << std::endl;
    if (this->selectedChild == -1)
        ShowStringWithPadding("Please enter the index of a command", 35, ' ');
    else
        ShowStringWithPadding("Press Up/Down/Enter to use menu", 31, ' ');
    ShowStringWithPadding("-OR-", 4, ' ');
    ShowStringWithPadding("press Escape to exit", 20, ' ');
}

void menu::GetMenuCMD()
{
    this->PutMenu();
    while (true)
    {
        char confirmation = getch();
        std::cout << std::endl
                  << std::flush;

        // If the key ESC was pressed, close the program
        if (confirmation == 27)
            exit(0);

        // Switch modes
        else if (confirmation == 'x' || confirmation == 'X')
        {
            // Change the modes of the selection
            this->selectedChild = this->selectedChild == -1 ? 0 : -1;
            this->PutMenu();
        }

        else if (selectedChild == -1)
        {
            // I.E. Selection mode is by Index

            /* Index is calculated by:
             * extracting 1 for the string-length
             * adding 1 for the Back cmd (if case)
             * adding the 48 ASCII offset for the numerical value
             */
            if (confirmation >= '0' && confirmation < this->childrenSize + 48)
            {
                // If the confirmation is a char between 0 and the index
                int currentIndex = confirmation - 48; // Char to Int
                if (this->children[currentIndex]->childrenSize == 0)
                {
                    if (this->log->getDebug())
                        std::cout << this->children[currentIndex]->name
                                  << " is empty"
                                  << std::endl;
                    this->children[currentIndex]->ExecuteFunction();
                }
                else
                {
                    // Set children's selection mode to Index
                    this->children[currentIndex]->selectedChild = -1;
                    this->children[currentIndex]->GetMenuCMD();
                }
                this->PutMenu();
                // return
            }
            else if (confirmation == this->childrenSize + 48 && this->parent != nullptr)
            {
                // Set parent's selection mode to Index
                this->parent->selectedChild = -1;
                // Return to end this function and return to the previous loop (main loop of the parent menu)
                return;
                break; // Double check if required
            }
            else
                std::cout << "Please enter a valid index..."
                          << std::endl;
        }
        else
        {
            // Arrow ASCII is made out of 2 chars
            // We check whether we should execute the cmd or skip the first char
            if (confirmation != -32)
            {
                // Enter is pressed
                if (confirmation == 13)
                {
                    // Check if the Back option was selected
                    if (this->selectedChild == this->childrenSize && this->parent != nullptr)
                    {
                        // Set parent's selection mode to Navigation
                        this->parent->selectedChild = 0;
                        // Return to end this function and return to the previous loop (main loop of the parent menu)
                        return;
                        break; // Double check if required
                    }
                    else
                    {
                        if (this->children[this->selectedChild]->childrenSize == 0)
                        {
                            if (this->log->getDebug())
                                std::cout << this->children[this->selectedChild]->name
                                          << " is empty"
                                          << std::endl;
                            this->children[this->selectedChild]->ExecuteFunction();
                        }
                        else
                        {
                            // Set children's selection mode to Navigation
                            this->children[this->selectedChild]->selectedChild = 0;
                            this->children[this->selectedChild]->GetMenuCMD();
                        }
                        this->PutMenu();
                        // return
                    }
                }
                // I.E. Selection mode is by Navigation
                else if (confirmation == 72)
                {
                    // Up arrow
                    if (selectedChild != 0)
                        selectedChild--;
                    else
                        selectedChild = this->childrenSize + (this->parent ? 0 : -1);
                    this->PutMenu();
                }
                else if (confirmation == 80)
                {
                    // Down arrow
                    if (selectedChild != this->childrenSize + (this->parent ? 0 : -1))
                        selectedChild++;
                    else
                        selectedChild = 0;
                    this->PutMenu();
                }
                else
                    std::cout << "Please only use the Up/Down arrows to navigate and Enter to confirm..."
                              << std::endl;
            }
        }
    }
}

void menu::ExecuteFunction()
{

    switch (this->functionIndex)
    {
    case 0: // * Start
        this->graphData->Solve();
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        if (this->graphData->isEmpty())
        {
            std::cout << "The graph has not yet been initialized. Continue with initializing the graph."
                      << std::endl
                      << "Press any key to continue...";
            getch();
            this->graphData->InitializeGraph(this->log);
        }
        this->graphData->Solve();
        this->graphData->WriteGraphSolution();
        std::cout << std::endl
                  << "Press any key to continue...";
        getch();
        break;
    case 1: // * Read data
        this->graphData->InitializeGraph(this->log);
        std::cout << "Data read successfully. Press any key to continue...";
        getch();
        break;
    case 2: // * Display data
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->DisplayGraphData();
        getch();
        break;
    case 3: // * Add node
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->AddNode();
        break;
    case 4: // * Delete node
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->DeleteNode();
        break;
    case 5: // * Add edge
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->AddEdge();
        break;
    case 6: // * Delete edge
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->DeleteEdge();
        break;
    case 7: // * Save data
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->SaveData();
        break;
    case 10: // * Restore default save location
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->RestoreDefaultSaveLocation();
        break;
    case 11: // * Custom save location
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->SetCustomSaveLocation();
        break;
    case 8: // * Custom load location
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->LoadDataFromCustomPath();
        break;
    case 9: // * Load from presets
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        this->graphData->GetPresetMenu();
        break;
    case 12: // * Toggle Debug Log
        // Set the debug to the opposite value
        this->log->setDebug(!this->log->getDebug());
        std::cout << "Debugging log set to "
                  << this->log->getDebug()
                  << std::endl
                  << "Press any key to continue..."
                  << std::endl;
        getch();
        break;
    case 13: // * Toggle console clear
        // Set the console clear to the opposite value
        this->log->setClearConsole(!this->log->getClearConsole());
        std::cout << "Clear console set to "
                  << this->log->getClearConsole()
                  << std::endl
                  << "Press any key to continue..."
                  << std::endl;
        getch();
        break;
    case 14: // * Help
        if (this->log->getClearConsole())
            this->log->ClearConsole();
        std::cout << "This application with a friendly interface has been created to:"
                  << std::endl
                  << "\t> Instantiate a graph and it's data."
                  << std::endl
                  << "\t> Get the shortest path between any 2 nodes."
                  << std::endl
                  << std::endl
                  << "Project realised by Ninel-Valentin Banica 2023"
                  << std::endl
                  << std::endl
                  << "Press any key to get back to the menu...";
        getch();
        break;
    case 15: // * Exit
        exit(0);
    default:
        std::cout << "Default type name encountered:"
                  << this->functionIndex
                  << std::endl;
        getch();
    }
}