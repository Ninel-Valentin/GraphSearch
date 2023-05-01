#include <iostream>
#include <fstream>
#include <string>

struct menuData
{
    char *name;
    int nameLength = 0;
    // Size initialized with 0 for menu entries which have no children
    int childrenSize = 0;
    int *childrenIds;
};

struct menuSet
{
    int size;
    menuData *data;
};

class menu
{
private:
    int selectedChild = -1; // If this has the value -1, it means we are not in selective mode, otherwise, it represents the index of the children
    char *name;
    int nameLength = 0;
    int childrenSize = 0; // Size initialized with 0 for menu entries which have no children
    menu *children;
    menu *parent;
    // bool isEmpty;
    loggingSystem *log; // This represents a reference to the logging system of the program
    static const char *dataPath;

public:
#pragma region Constructors prototypes
    menu();
    menu(const menu &);
    menu(int, char *, int, loggingSystem *);
    ~menu();
#pragma endregion
#pragma region Constructors Functions prototypes
    static menuSet *ReadMenuSetFromFile(loggingSystem *);
    static menu *InstantiateMenu(loggingSystem *);
#pragma endregion
#pragma region Modifiers prototypes
#pragma endregion
#pragma region Service prototypes
    void PutMenu();
    void GetMenuCMD();
    void ExecuteFunctionFromName();

#pragma endregion
};
const char *menu::dataPath = "storage/menuSet.txt";
#pragma region Helper Functions prototypes
int CountOfCharInString(std::string, char);
void AddChildrenIndexes(std::string, menuData &);
int paddingOfString(int);
int paddingOfString(int, int);
// This method returns a string which adds the the provided filler to both sides of the provided string as a padding until the final string reaches the length of 50
std::string stringWithPadding(const char *, int, char);
// This method returns a string which adds the the provided filler to both sides of the provided string as a padding until the final string reaches the length of 50 and adds the selection identifier if isSelected = true
std::string stringWithPadding(const char *, int, char, bool);
#pragma endregion

#pragma region Constructors definitions
menu::menu()
{
}
menu::menu(const menu &_m)
{
    delete[] this->name;
    this->nameLength = _m.nameLength;
    this->name = new char[(this->nameLength + 1)];

    // Copy the name of the menu
    for (int i = 0; i < this->nameLength; i++)
        this->name[i] = _m.name[i];
    this->childrenSize = _m.childrenSize;
    this->children = new menu[this->childrenSize];

    // Copy the children of the menu
    for (int i = 0; i < this->childrenSize; i++)
        this->children[i] = _m.children[i];

    this->parent = new menu();
}
menu::menu(int _nameLength, char *_name, int _childrenSize, loggingSystem *_log)
{
    delete[] this->name;
    this->nameLength = _nameLength;
    this->name = new char[this->nameLength + 1];

    // Copy the name of the menu
    for (int i = 0; i < this->nameLength; i++)
        this->name[i] = _name[i];

    this->childrenSize = _childrenSize;
    this->children = new menu[_childrenSize];

    // Instantiate empty menus
    for (int i = 0; i < this->childrenSize; i++)
        this->children[i] = menu();

    this->parent = new menu();

    // Add a reference to the logging system of the program
    this->log = _log;
}
menu::~menu()
{
}
#pragma endregion
#pragma region Constructors Functions definitions
menuSet *menu::ReadMenuSetFromFile(loggingSystem *log)
{
    // Declare the input file stream and open the file.
    std::ifstream MenuSetFile;
    MenuSetFile.open(dataPath);
    // Initialize a string varia ble for the text file lines
    std::string dataLine;

    // Read the first line as it contains the number of menu entries in the file
    std::getline(MenuSetFile, dataLine);

    // Instantiate the set that will be returned
    menuSet *set = new menuSet;

    // Read the length of the array from the first row from the file
    set->size = std::stoi(dataLine); // std::stoi -> String to Int conversion
    set->data = new menuData[set->size];

    if (log->getDebug())
        std::cout << "DEBUG: Reading menus from the file..."
                  << std::endl;

    int menuIndex = 0;
    // Check if the file is open, otherwise show a message and stop the program
    if (MenuSetFile.is_open())
    {
        // Iterate the file data until we find the end of file
        while (!MenuSetFile.eof())
        {
            // Read the next line
            std::getline(MenuSetFile, dataLine);

            // Check to skip empty rows
            if (dataLine.length() > 0)
            {
                // Instantiate the name string which will be given to the menu
                std::string croppedName;

                // Each row is built as such: _NAME_>_CHILD1_|_CHILD2_|_CHILD3_|...
                int delimiterPos = dataLine.find('>'); // The delimiter of the name/children

                // Clear the current name memory allocation
                delete[] set->data[menuIndex].name;
                // Instantiate the new required memory and store the name -> has delimiter ? length is delimiter : entire length  + 1(end of string)
                set->data[menuIndex].nameLength = (delimiterPos != -1 ? delimiterPos : dataLine.length()) + 1;

                // Instantiate the length of the name with the length of required string + 1
                set->data[menuIndex].name = new char[set->data[menuIndex].nameLength];

                // Check if the delimiter position is not the 'Not found' response of the .find() function
                if (delimiterPos != -1)
                {
                    // Split name from children
                    croppedName = dataLine.substr(0, delimiterPos).c_str();

                    // Children indexes Data string
                    std::string indexesStr = dataLine.substr(delimiterPos + 1, dataLine.length() - delimiterPos - 1);

                    // Array of childrens' ids and it's length
                    set->data[menuIndex].childrenSize = CountOfCharInString(indexesStr, '|');

                    delete[] set->data[menuIndex].childrenIds;
                    set->data[menuIndex].childrenIds = new int[set->data[menuIndex].childrenSize];

                    // Add the childrenIndexes by reference to the data object
                    AddChildrenIndexes(indexesStr, set->data[menuIndex]);
                }
                else
                {
                    // No children = full string is name
                    croppedName = dataLine.c_str();
                }

                // Copy the string to the name
                for (int i = 0; i < croppedName.length(); i++)
                    set->data[menuIndex].name[i] = croppedName.at(i);
                // Copying from a string does not add the end of string character, which means we add it manually
                set->data[menuIndex].name[croppedName.length()] = '\0';

                if (log->getDebug())
                    std::cout << menuIndex
                              << ": "
                              << set->data[menuIndex].name
                              << std::endl;

                // Increment counter
                menuIndex++;
            }
        }
    }
    else
    {
        std::cout << "Menu set file not found... Stopping process"
                  << std::endl;
        exit(1);
    }

    return set;
}

menu *menu::InstantiateMenu(loggingSystem *log)
{
    // Instantiate the menuSet data from the file, which will then be parsed into an object.
    menuSet *menuDataSet = menu::ReadMenuSetFromFile(log);
    if (log->getDebug())
    {
        std::cout << "DEBUG: Retrieved the menuSet data successfully. Parsing it into menu data..."
                  << std::endl
                  << "Press any key to continue!"
                  << std::endl;
        getch();
    }
    // Reset and instantiate the array of menu items as long as the menuDataSet array
    menu *_menu = new menu[menuDataSet->size];

    // Instantiate a menu for each entry
    for (int i = 0; i < menuDataSet->size; i++)
        _menu[i] = *(new menu(menuDataSet->data[i].nameLength, menuDataSet->data[i].name, menuDataSet->data[i].childrenSize, log));

    // Iterate the menus and link the children with the parents
    for (int i = 0; i < menuDataSet->size; i++)
    {
        int count = _menu[i].childrenSize;
        // The menu is or is not empty I.E. does execute a function or only opens a new submenu
        for (int j = 0; j < count; j++)
        {
            // Id of the j-th children
            int id = menuDataSet->data[i].childrenIds[j];

            // Append the children to the parent's children array
            _menu[i].children[j] = _menu[id];
            // Link the parent to the child
            // delete _menu[id].parent;
            _menu[id].parent = &_menu[i];
        }
    }

    // Reset the main menu's parent to nullptr
    delete _menu->parent;
    _menu->parent = nullptr;
}
#pragma endregion
#pragma region Modifiers prototypes
#pragma endregion
#pragma region Service definitions
void menu::PutMenu()
{
    // Check if the loggingSystem requires the console to be cleared
    if (this->log->getClearConsole())
        this->log->ClearConsole();

    std::cout << stringWithPadding("(PRESS [x] TO SWITCH MENU TYPE)", 31, '-')
              << std::endl;

    // Calculate the padding required for each name/message to center them
    std::cout << stringWithPadding(this->name, this->nameLength, '-');

    for (int i = 0; i < this->childrenSize; i++)
        std::cout << i
                  << ": "
                  << stringWithPadding(this->children[i].name,
                                       this->children[i].nameLength + 2 * (2 + (i < 10 ? 1 : 2)), // the length of the string + the length of "i: " * 2 (for each side's padding)
                                       ' ',
                                       this->selectedChild == i);

    // Back command for nested menus
    if (this->parent)
        std::cout << this->childrenSize
                  << ": "
                  << stringWithPadding("Back",
                                       4 + 2 * (2 + (this->childrenSize < 10 ? 1 : 2)), // the length of the string + the length of "i: " * 2 (for each side's padding),
                                       ' ',
                                       this->selectedChild == this->childrenSize);

    std::cout << stringWithPadding(this->name, this->nameLength, '-')
              << (this->selectedChild == -1 ? stringWithPadding("Please enter the index of a command", 35, ' ')
                                            : stringWithPadding("Press Up/Down/Enter to use menu", 31, ' '))
              << stringWithPadding("-OR-", 4, ' ')
              << stringWithPadding("press Escape to exit", 20, ' ');
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
                if (this->children[currentIndex].childrenSize == 0)
                {
                    if (this->log->getDebug())
                        std::cout << this->children[currentIndex].name
                                  << " is empty"
                                  << std::endl;
                    this->children[currentIndex].ExecuteFunctionFromName();
                }
                else
                {
                    // Set children's selection mode to Index
                    this->children[currentIndex].selectedChild = -1;
                    this->children[currentIndex].GetMenuCMD();
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
                        if (this->children[this->selectedChild].childrenSize == 0)
                        {
                            if (this->log->getDebug())
                                std::cout << this->children[this->selectedChild].name
                                          << " is empty"
                                          << std::endl;
                            this->children[this->selectedChild].ExecuteFunctionFromName();
                        }
                        else
                        {
                            // Set children's selection mode to Navigation
                            this->children[this->selectedChild].selectedChild = 0;
                            this->children[this->selectedChild].GetMenuCMD();
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
                    this->PutMenu();
                }
                else if (confirmation == 80)
                {
                    // Down arrow
                    if (selectedChild != this->childrenSize + (this->parent ? 0 : -1))
                        selectedChild++;
                    this->PutMenu();
                }
                else
                    std::cout << "Please only use the Up/Down arrows to navigate and Enter to confirm..."
                              << std::endl;
            }
        }
    }
}

void menu::ExecuteFunctionFromName()
{
    std::string _nameString = this->name;
    std::map<std::string, MenuNames> _nameSwatch;
    InitializeMenuNameMap(_nameSwatch);
    if (this->log->getDebug())
        std::cout << "DEBUG: "
                  << _nameString
                  << std::endl
                  << _nameSwatch[_nameString]
                  << std::endl;

    switch (_nameSwatch[_nameString])
    {
    case Start_algorithm:
        StartAlgorithm();
        break;
    case Read_data:
        ReadData();
        break;
    case Display_data:
        DisplayData();
        break;
    case Add_node:
        AddNode();
        break;
    case Delete_node:
        DeleteNode();
        break;
    case Add_link:
        AddLink();
        break;
    case Delete_link:
        DeleteLink();
        break;
    case Erase_data:
        EraseData();
        break;
    case Custom_location:
        CustomLocation();
        break;
    case Default_location:
        DefaultLocation();
        break;
    case Toggle_Debug:
        // Set the debug to the opposite value
        this->log->setDebug(!this->log->getDebug());
        std::cout << "Debugging log set to "
                  << this->log->getDebug()
                  << std::endl
                  << "Press any key to continue..."
                  << std::endl;
        getch();
        break;
    case Toggle_console_clear:
        // Set the console clear to the opposite value
        this->log->setClearConsole(!this->log->getClearConsole());
        std::cout << "Clear console set to "
                  << this->log->getClearConsole()
                  << std::endl
                  << "Press any key to continue..."
                  << std::endl;
        getch();
        break;
    case Exit:
        exit(0);
    default:
        std::cout << "Default type name encountered:"
                  << _nameSwatch[_nameString]
                  << std::endl;
        getch();
    }
}

#pragma endregion
#pragma region Helper Functions definitions
int CountOfCharInString(std::string str, char ch)
{
    int count = 0;
    for (int i = 0; i < str.length(); i++)
        if (str.at(i) == ch)
            count++;
    return count;
}

void AddChildrenIndexes(std::string indexesString, menuData &data)
{
    data.childrenIds = new int[data.childrenSize];

    // Initialize an empty string which will be a temporary IDs storage
    std::string temp = "";
    for (int i = 0, j = 0; i < indexesString.length(); i++)
    {
        // IF the current char is the delimiter, append the id to the array and reset the string
        if (indexesString.at(i) == '|')
        {
            data.childrenIds[j++] = std::stoi(temp); // std::stoi -> String to Int conversion
            temp = "";
        }
        // ELSE append the current character to the temporary string
        else
            temp += indexesString.at(i);
    }
}

int paddingOfString(int strLength)
{
    return paddingOfString(strLength, 0);
}
// This function takes as argument the length of the message it will add the padding to
int paddingOfString(int strLength, int ignoreCount)
{
    // Max line char for logging
    int lineLength = 50;
    return ((lineLength - strLength) / 2 - ignoreCount);
}

std::string stringWithPadding(const char *str, int strLength, char filler)
{
    return stringWithPadding(str, strLength, filler, false);
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
#pragma endregion
