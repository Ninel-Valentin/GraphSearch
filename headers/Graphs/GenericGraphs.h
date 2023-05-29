#include <cstring>
#include <iostream>
#include <fstream>

#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H
#include "../loggingSystem.h"
#endif

#ifndef ARRAY_H
#define ARRAY_H
#include "Array.h"
#endif

class GenericGraph
{
private:
    int src = 0;
    Array *nodes;
    int **edges;
    Array
        *predecessorArr,
        *lengthArr;
    bool useCustomSaveLocation = false;

public:
    GenericGraph();
    ~GenericGraph();
    bool isEmpty();
    void InitializeGraph(loggingSystem *);
    // This function provides the position array as the first param, and the length array as the second param
    void Solve();
    void WriteGraphSolution();
    void DisplayGraphData();
    void DisplayNodesData(bool);
    void DisplayEdgesData(bool);
    void AddNode();
    void DeleteNode();
    void AddEdge();
    void DeleteEdge();
    void ShowPresetMenu();
    void GetPresetMenu();
    void LoadFromData(int *, int);
    void LoadDataFromPath(char *);
    void LoadDataFromCustomPath();
    int *CreateDataSet(int &);
    void SaveData();
    void SetCustomSaveLocation();
    void RestoreDefaultSaveLocation();
    static const char *presetPath;
    static const char *defaultSavePath;
    char *customSavePath;
};
const char *GenericGraph::presetPath = "storage/DataSamples/Sample";
const char *GenericGraph::defaultSavePath = "storage/saveFiles/";
// Check if the file can be loaded I.E. if the data inside matches the correct format
bool CheckFormatOfDataSet(int *, int);
char *GetFileNameFromConsole();
char *GetCustomPathFromConsole();

GenericGraph::GenericGraph()
{
    this->nodes = new Array();
}
GenericGraph::~GenericGraph()
{
    if (this->nodes->length() > 0)
    {
        for (int i = 0; i < this->nodes->length(); i++)
            delete[] edges[i];
        delete[] edges;
        delete nodes;
        delete[] predecessorArr;
        delete[] lengthArr;
    }
}

bool GenericGraph::isEmpty()
{
    return this->nodes->length() == 0;
}

void GenericGraph::InitializeGraph(loggingSystem *log)
{
    // If this is a reInitialization, first clear the data which does not reset below already
    if (nodes->length() != 0)
    {
        for (int i = 0; i < this->nodes->length(); i++)
            delete[] edges[i];
        delete[] edges;
    }

    if (log->getClearConsole())
        system("CLS");
    ShowStringWithPadding("Reading graph data", 18, '-');
    std::cout << "Enter the number of nodes in the graph: ";
    int x;
    std::cin >> x;
    this->nodes->resize(x);

    // Initialize the nodes with the default values;
    for (int i = 0; i < this->nodes->length(); i++)
        nodes->set(i, i);

    // loop to check if the provided source node is part of the graph
    while (true)
    {
        std::cout << "Please pick a source node [0-"
                  << this->nodes->length() - 1
                  << "]: ";
        std::cin >> this->src;
        // If the src is part of the current nodes, break the loop
        if (this->src < this->nodes->length() &&
            this->src >= 0)
            break;
        std::cout << "Incorrect value! Retrying..."
                  << std::endl;
    }

    int edgesCount = 0;
    while (true)
    {
        std::cout
            << "Please enter the number of edges in the graph: ";
        std::cin >> edgesCount;
        int maxEdges = this->nodes->length() * (this->nodes->length() - 1) / 2;
        if (edgesCount <= maxEdges)
            break;
        else
            std::cout << "You can not have more than "
                      << maxEdges
                      << " edges in this graph."
                      << std::endl;
    }
    this->edges = new int *[this->nodes->length()];
    for (int i = 0; i < this->nodes->length(); i++)
        this->edges[i] = new int[this->nodes->length()];

    // Instantiate the adjacency matrix
    for (int i = 0; i < this->nodes->length(); i++)
        for (int j = 0; j < this->nodes->length(); j++)
            this->edges[i][j] = 0;

    // Read the edges
    for (int i = 0; i < edgesCount; i++)
    {
        int a, b;
        std::cout << "[Edge "
                  << i + 1
                  << " out of " << edgesCount
                  << "]: Insert the 2 points forming the edge:"
                  << std::endl;
        std::cin >> a >> b;

        // Check if the nodes are different
        if (a == b)
        {
            std::cout << "You can't create an edge between the same 2 points... Retry:"
                      << std::endl;
            i--;
        }
        else if (a < 0 || a >= this->nodes->length())
        {
            std::cout << "The start node is not part of the graph... Retry:"
                      << std::endl;
            i--;
        }
        else if (b < 0 || b >= this->nodes->length())
        {
            std::cout << "The end node is not part of the graph... Retry:"
                      << std::endl;
            i--;
        }
        else if (this->edges[a][b] == 1)
        {
            std::cout << "You already instantiated an edge here... Retry:"
                      << std::endl;
            i--;
        }
        else
        {
            this->edges[a][b] = 1;
            this->edges[b][a] = 1;
        }
    }
}

void GenericGraph::Solve()
{

    int knownNodesIndex = 1,
        unknownNodesIndex = this->nodes->length() - 1,
        analyzedNodesIndex = 0,
        lengthIndex = 0;

    // knownNodes of len 1 (only the src)
    // unknownNodes of nodes length because we insert all nodes, after which we remove the src
    // analyzed of length 0
    Array *knownNodes = new Array(1),
          *unknownNodes = new Array(this->nodes->length()),
          *analyzedNodes = new Array();

    // We do not delete this as this function always instantiates new arrays.
    this->lengthArr = new Array(this->nodes->length(), -1);
    this->predecessorArr = new Array(this->nodes->length(), -1); // All values initialized with 0

    // Starting from the source
    knownNodes->set(0, this->src);

    // Copy all nodes and remove the source
    for (int i = 0; i < this->nodes->length(); i++)
        unknownNodes->set(i, this->nodes->getValue(i));
    unknownNodes->removeByValue(this->src);

    while (*analyzedNodes != *this->nodes)
    {
        while (knownNodes->length() != 0)
        {

            // Select the oldest node from knownNodes (I.E. the first node)
            int x = knownNodes->first(),
                y;

            for (y = 0; y < this->nodes->length(); y++)
                // For all edges from X to an unknown node:
                if (this->edges[x][y] == 1 && unknownNodes->getIndex(y) != -1)
                {
                    knownNodes->add(y);
                    unknownNodes->removeByValue(y);
                    this->predecessorArr->set(y, x);
                    this->lengthArr->set(y, this->lengthArr->getValue(x) + 1);
                }

            knownNodes->removeByValue(x);
            analyzedNodes->add(x);
        }
        if (unknownNodes->length() != 0)
        {
            // Select a new source node
            int newSource = unknownNodes->first();
            knownNodes->add(newSource);
            unknownNodes->removeByValue(newSource);
        }

        // Sort these before comparing them in the while -> this does not affect the algorithm
        analyzedNodes->sort();
        this->nodes->sort();
    }
}

void GenericGraph::WriteGraphSolution()
{
    ShowStringWithPadding("Solution data", 13, '-');
    std::cout << std::endl
              << "Source is: "
              << this->src
              << std::endl
              << "\tNode\t|\tDistance(from source)\t|\tPath";

    for (int i = 0; i < this->nodes->length(); i++)
    {
        std::cout << std::endl
                  << "\t"
                  << i
                  << "\t|\t\t";
        if (this->lengthArr->getValue(i) == -1)
            std::cout << "No path";
        else
            std::cout << this->lengthArr->getValue(i) + 1;
        std::cout << "\t\t|\t";

        int crrIndex = this->predecessorArr->getValue(i);
        if (this->src == i)
            std::cout << "This is the source node "
                      << this->src;
        else if (crrIndex == -1)
            std::cout << "No available path from source node "
                      << this->src
                      << " to "
                      << i;
        else
        {
            // Copy the predecessors in reverse order to another array.
            int *reversedList = new int[this->predecessorArr->length()];
            int k = 0;
            // Create the reversed list from the point until we get to the A source node
            while (crrIndex != this->src && crrIndex != -1)
            {
                reversedList[k++] = crrIndex;
                crrIndex = this->predecessorArr->getValue(crrIndex);
            }
            if (crrIndex == -1)
                k--;
            else
                reversedList[k] = crrIndex;

            for (; k >= 0; k--)
                std::cout << reversedList[k]
                          << "->";
            std::cout << i;

            if (crrIndex == -1)
                std::cout << " (Swapped source)";

            delete[] reversedList;
        }
    }
}

void GenericGraph::DisplayGraphData()
{
    ShowStringWithPadding("Graph data", 10, '-');
    this->DisplayNodesData(false);
    this->DisplayEdgesData(false);
}

void GenericGraph::DisplayNodesData(bool showHeader)
{
    if (this->nodes->length() == 0)
        std::cout << "There is no nodes data read for the graph"
                  << std::endl;
    else
    {
        if (showHeader)
            ShowStringWithPadding("Nodes data", 10, '-');

        std::cout << std::endl
                  << "Graph nodes ["
                  << this->nodes->length()
                  << " total node(s)]: ";
        for (int i = 0; i < this->nodes->length(); i++)
            std::cout << nodes->getValue(i)
                      << " ";
        std::cout << std::endl;
    }
}

void GenericGraph::DisplayEdgesData(bool showHeader)
{
    if (this->nodes->length() == 0)
        std::cout << "There is no edge data read for the graph"
                  << std::endl;
    else
    {
        if (showHeader)
            ShowStringWithPadding("Edges data", 10, '-');

        // Get the count of the edges as this is not a stored value.
        int edgesLength = 0;
        for (int i = 0; i < this->nodes->length(); i++)
            for (int j = 0; j < this->nodes->length(); j++)
                if (this->edges[i][j])
                    edgesLength++;

        std::cout << std::endl
                  << "Graph edges ["
                  << edgesLength
                  << " total edge(s)]: ";

        for (int i = 0; i < this->nodes->length() - 1; i++)     // < len-1 I.E. Last row is not required for this check
            for (int j = i + 1; j < this->nodes->length(); j++) // Start from i+1 I.E. above the main diagonal
                if (this->edges[i][j])
                    std::cout << i
                              << "->"
                              << j
                              << " ";
    }
}

void GenericGraph::AddNode()
{
    do
    {
        int newNode = this->nodes->length();
        ShowStringWithPadding("Add a new node", 14, '-');
        std::cout << "The new node will be: "
                  << newNode
                  << std::endl;
        ShowStringWithPadding("Add a new node", 14, '-');

        if (GetQuestionAnswer("Would you like to proceed with the change? (Y/N)"))
        {
            this->nodes->add(newNode);

            // Resize the edges matrix
            int **aux = new int *[this->nodes->length()];
            for (int i = 0; i < this->nodes->length(); i++)
                aux[i] = new int[this->nodes->length()];

            // Copy the edges matrix to the aux matrix;
            for (int i = 0; i < this->nodes->length() - 1; i++)
                for (int j = 0; j < this->nodes->length() - 1; j++)
                    aux[i][j] = this->edges[i][j];

            // Add the 0 value to the new row and column
            for (int i = 0; i < this->nodes->length(); i++)
            {
                aux[i][this->nodes->length() - 1] = 0;
                aux[this->nodes->length() - 1][i] = 0;
            }

            // Delete the old edges matrix
            for (int i = 0; i < this->nodes->length() - 1; i++)
                delete this->edges[i];
            delete[] this->edges;

            // Reinitialize the edges matrix with the new size
            this->edges = new int *[this->nodes->length()];
            for (int i = 0; i < this->nodes->length(); i++)
                this->edges[i] = new int[this->nodes->length()];

            // Copy the aux values into the edges matrix
            for (int i = 0; i < this->nodes->length(); i++)
                for (int j = 0; j < this->nodes->length(); j++)
                    this->edges[i][j] = aux[i][j];

            // Delete the aux matrix
            for (int i = 0; i < this->nodes->length(); i++)
                delete aux[i];
            delete[] aux;

            std::cout << "Changes made successfully!"
                      << std::endl;
        }
        else
            std::cout << "No changes had been made!"
                      << std::endl;
    } while (GetQuestionAnswer("Would you like to do any more changes? (Y/N)"));

    std::cout << "Press any key to continue...";
    getch();
}

void GenericGraph::DeleteNode()
{
    // Check if there is data in the graph
    if (this->nodes->length() > 0)
    {
        ShowStringWithPadding("!WARNING!", 9, '-');
        ShowStringWithPadding("Deleting a node will shift", 26, ' ');
        ShowStringWithPadding("the higher nodes one value down", 31, ' ');
        ShowStringWithPadding("(This modifies the name, not the data)", 38, ' ');
        ShowStringWithPadding("!WARNING!", 9, '-');
        std::cout << std::endl;
        this->DisplayNodesData(true);
        std::cout << std::endl;
        ShowStringWithPadding("Pick a node to delete", 21, '-');
        ShowStringWithPadding("OR", 2, ' ');
        ShowStringWithPadding("Enter -1 to stop reading for a value", 36, ' ');
        std::cout << std::endl;

        int node;
        do
        {
            std::cout << "Enter a node: ";
            std::cin >> node;
            // Check if we require to stop the loop
            if (node == -1)
                break;
            if (GetQuestionAnswer("Would you like to proceed with the change? (Y/N)"))
            {
                if (nodes->getIndex(node) != -1)
                {
                    // Remove the node from the nodes array AND decrement the length
                    // nodes->removeByValue(node);
                    // Remove the node and shift the values from the right, one value to the left I.E. remove last
                    nodes->filo();
                    std::cout << "Node removed successfully!"
                              << std::endl;
                    // Remove all edges for this node
                    for (int i = 0; i < this->nodes->length(); i++)
                    {
                        this->edges[i][node] = 0;
                        this->edges[node][i] = 0;
                    }
                    // Resize the edges matrix
                    int **aux = new int *[this->nodes->length()];
                    for (int i = 0; i < this->nodes->length(); i++)
                        aux[i] = new int[this->nodes->length()];

                    for (int i = 0, rAdd = 0, cAdd = 0; i < this->nodes->length() + 1; i++)
                    {
                        // Skip node's row on copy
                        if (i == node)
                        {
                            // Increment the row addition
                            rAdd++;
                            continue;
                        }
                        for (int j = 0; j < this->nodes->length() + 1; j++)
                        {
                            // Skip node's column on copy
                            if (j == node && cAdd == 0)
                            {
                                // Increment the column addition
                                cAdd++;
                                continue;
                            }
                            aux[i - rAdd][j - cAdd] = this->edges[i][j];
                        }
                    }

                    // Delete the old edges matrix
                    for (int i = 0; i < this->nodes->length() + 1; i++)
                        delete this->edges[i];
                    delete[] this->edges;

                    // Reinitialize the edges matrix with the new size
                    this->edges = new int *[this->nodes->length()];
                    for (int i = 0; i < this->nodes->length(); i++)
                        this->edges[i] = new int[this->nodes->length()];

                    // Copy the aux values into the edges matrix
                    for (int i = 0; i < this->nodes->length(); i++)
                        for (int j = 0; j < this->nodes->length(); j++)
                            this->edges[i][j] = aux[i][j];

                    // Delete the aux matrix
                    for (int i = 0; i < this->nodes->length(); i++)
                        delete aux[i];
                    delete[] aux;

                    // Resolve the graph for the new data
                    this->Solve();
                    // Exit the while loop
                    break;
                }
                else
                    std::cout << "Node does not exist...";
            }
        } while (this->nodes->getIndex(node) == -1);
    }
    else
        std::cout << "No data read for the graph yet!"
                  << std::endl;

    std::cout << "Press any key to continue...";
    getch();
}

void GenericGraph::AddEdge()
{
    do
    {
        // Check if there is data in the graph
        if (this->nodes->length() > 0)
        {
            this->DisplayEdgesData(true);
            std::cout << std::endl;
            ShowStringWithPadding("Pick an edge to add", 21, '-');
            std::cout << std::endl;

            int x, y;
            do
            {
                std::cout << "Write the edge's ends:"
                          << std::endl
                          << "From: ";
                std::cin >> x;
                std::cout << "To: ";
                std::cin >> y;
                if (x < 0 || x >= this->nodes->length() ||
                    y < 0 || y >= this->nodes->length())
                    std::cout << "The ends should be nodes part of the graph! Retrying..."
                              << std::endl;
            } while (x < 0 || x >= this->nodes->length() ||
                     y < 0 || y >= this->nodes->length());
            if (GetQuestionAnswer("Would you like to proceed with the change? (Y/N)"))
                if (this->edges[x][y] == 1)
                    std::cout << "This edge already exists."
                              << std::endl;
                else
                {
                    this->edges[x][y] = 1;
                    std::cout << "Edge added successfully!"
                              << std::endl;
                }
            else
                std::cout << "No changes were!"
                          << std::endl;
        }
        else
            std::cout << "Node does not exist...";

    } while (GetQuestionAnswer("Would you like to do any more changes? (Y/N)"));
    std::cout << "Press any key to continue...";
    getch();
}

void GenericGraph::DeleteEdge()
{

    // Check if there is data in the graph
    if (this->nodes->length() > 0)
    {
        do
        {
            this->DisplayEdgesData(true);
            std::cout << std::endl;
            ShowStringWithPadding("Pick an edge to delete", 24, '-');
            std::cout << std::endl;

            int x, y;
            do
            {
                std::cout << "Write the edge's ends:"
                          << std::endl
                          << "From: ";
                std::cin >> x;
                std::cout << "To: ";
                std::cin >> y;
                if (x < 0 || x >= this->nodes->length() ||
                    y < 0 || y >= this->nodes->length())
                    std::cout << "The ends should be nodes part of the graph! Retrying..."
                              << std::endl;
            } while (x < 0 || x >= this->nodes->length() ||
                     y < 0 || y >= this->nodes->length());
            if (GetQuestionAnswer("Would you like to proceed with the change? (Y/N)"))
                if (this->edges[x][y] == 1)
                {
                    this->edges[x][y] = 0;
                    std::cout << "Edge deleted successfully!"
                              << std::endl;
                }
                else
                    std::cout << "This edge does not exists."
                              << std::endl;

        } while (GetQuestionAnswer("Would you like to do any more changes? (Y/N)"));
        std::cout << "Press any key to continue...";
    }
    else
        std::cout << "No data read for the graph yet...";
    getch();
}

void GenericGraph::ShowPresetMenu()
{
    ShowStringWithPadding("Load a preset", 14, '-');
    std::cout << std::endl
              << "Choose one of the following presets:"
              << std::endl;
    for (int i = 1; i <= samplesCount; i++)
    {
        char *name = new char[9]{};
        strcat(name, "Sample");

        // Create a temp char array
        char *temp = new char[3];

        if (i <= 9)
        {
            itoa(0, temp, 10);
            strcat(name, temp);
        }

        // Use itoa to cast the int i into a char array
        itoa(i, temp, 10); // number, array, base
        strcat(name, temp);

        ShowStringWithPadding(name, 8, ' ');
    }
    ShowStringWithPadding("Load a preset", 14, '-');
    ShowStringWithPadding("Enter the number of a preset", 29, ' ');
    ShowStringWithPadding("OR", 2, ' ');
    ShowStringWithPadding("Type -1 to go back", 19, ' ');
    std::cout << std::endl;
}

void GenericGraph::GetPresetMenu()
{
    ShowPresetMenu();
    int nr;
    do
    {
        std::cout << "Insert the number of the Sample file to load: ";
        std::cin >> nr;

        if (nr < -1 || nr > samplesCount)
            std::cout << "That is not a sample number. Retrying..."
                      << std::endl;
        else if (nr == -1)
        {
            std::cout << "Exit required. Press any key to continue..."
                      << std::endl;
            getch();
            return;
        }
        else
        {
            char *finalPath = new char[33]{};
            strcat(finalPath, "storage/DataSamples/Sample");
            // Concatenate the path: "storage/DataSamples/Sample" + ("0n" || "nn") + ".txt"

            // Create a temp char array
            char *temp = new char[3];

            if (nr <= 9)
            {
                itoa(0, temp, 10);
                strcat(finalPath, temp);
            }

            // Use itoa to cast the int i into a char array
            itoa(nr, temp, 10); // number, array, base
            strcat(finalPath, temp);
            strcat(finalPath, ".txt");

            this->LoadDataFromPath(finalPath);
        }

        // Repeat until the number read is -1 or a sample file
    } while (nr < -1 || nr > samplesCount);
}

void GenericGraph::LoadFromData(int *dataArr, int dataLen)
{
    // Check if the data has the correct format before deleting current data
    if (CheckFormatOfDataSet(dataArr, dataLen))
    {
        // Clear the current data (if there is any)
        if (this->nodes->length() > 0)
        {
            for (int i = 0; i < this->nodes->length(); i++)
                delete[] edges[i];
            delete[] edges;
            delete nodes;
        }
        // Instantiate a cursor to go through the dataArr
        int cursor = 0;

        // First value is the number of nodes, increment cursor
        this->nodes = new Array(dataArr[cursor++]);

        // Initialize the nodes with the default values;
        for (int i = 0; i < this->nodes->length(); i++)
            nodes->set(i, i);

        // Store the source and increment the cursor
        this->src = dataArr[cursor++];

        // Next value is the edges count, increment the counter
        // int edgesCount = dataArr[cursor++];
        cursor++;

        // Initialize the edges matrix
        this->edges = new int *[this->nodes->length()];
        for (int i = 0; i < this->nodes->length(); i++)
            this->edges[i] = new int[this->nodes->length()];

        // Instantiate the adjacency matrix with default value 0
        for (int i = 0; i < this->nodes->length(); i++)
            for (int j = 0; j < this->nodes->length(); j++)
                this->edges[i][j] = 0;

        // Next values are pairs of start-finish edges' ends
        while (cursor < dataLen)
        {
            int start = dataArr[cursor++],
                finish = dataArr[cursor++];

            this->edges[start][finish] = 1;
            this->edges[finish][start] = 1;
        }
    }
}

void GenericGraph::LoadDataFromPath(char *path)
{
    std::ifstream PresetFile;
    PresetFile.open(path);

    if (PresetFile)
    {
        if (PresetFile.is_open())
        {
            int len;
            // Read the first object from the file as it represents the length of entries in the file
            PresetFile >> len;
            // Instantiate the data array, the reader int and a counter index
            int x,
                index = 0,
                *dataArray = new int[len];

            // Iterate the file while we can read more data AND we have not passed the limit specified at the start in len
            while (PresetFile >> x && index < len)
                dataArray[index++] = x;

            PresetFile.close();

            this->LoadFromData(dataArray, len);
            std::cout << "Data loaded successfully! Displaying data:"
                      << std::endl;
            this->DisplayGraphData();
        }
        else
            std::cout << "File could not be openned... Check path of Sample file: "
                      << path
                      << std::endl;
    }
    else
        std::cout << "File could not be found... Check path of Sample file: "
                  << path
                  << std::endl;
    getch();
}

void GenericGraph::LoadDataFromCustomPath()
{
    ShowStringWithPadding("Load from custom location", 26, '-');
    std::cout << "Enter the custom location of the file (including the name file and extension):"
              << std::endl;

    this->LoadDataFromPath(GetCustomPathFromConsole());
    std::cout << "Done!";
}

int *GenericGraph::CreateDataSet(int &entriesCount)
{
    // Calculate how many entries the file will have
    entriesCount = 3;   // Start with 3 from: Nodes count + src + edges count
    int edgesCount = 0; // Instantiate the edges counter
    // Iterate just the half above the main diagonal of the matrix
    for (int i = 0; i < this->nodes->length() - 1; i++)
        for (int j = i + 1; j < this->nodes->length(); j++)
            if (this->edges[i][j] == 1)
                edgesCount++;

    // Add 2 * edgesCount (2 node ends for each edge) to the entriesCount
    entriesCount += edgesCount * 2;

    // Instantiate the array of entries, with length + 1 for the entriesCount itself at the start of the file
    int *entries = new int[entriesCount + 1],
        // Instantiate a cursor to iterate the entries
        cursor = 0;

    entries[cursor++] = entriesCount;
    entries[cursor++] = this->nodes->length();
    entries[cursor++] = this->src;
    entries[cursor++] = edgesCount;

    for (int i = 0; i < this->nodes->length() - 1; i++)
        for (int j = i + 1; j < this->nodes->length(); j++)
            if (this->edges[i][j] == 1)
            {
                entries[cursor++] = i;
                entries[cursor++] = j;
            }
    return entries;
}

void GenericGraph::SaveData()
{
    int count;
    int *entries = this->CreateDataSet(count);

    do
    {
        char *fileName = GetFileNameFromConsole();
        char *finalPath = new char[256]{};
        strcat(finalPath, this->useCustomSaveLocation ? this->customSavePath : this->defaultSavePath);
        strcat(finalPath, fileName);
        strcat(finalPath, ".txt");

        std::fstream SaveFile;
        SaveFile.open(finalPath, std::ios::in);

        if (SaveFile)
        {
            // File already exists
            std::cout << "File already exists."
                      << std::endl;
            if (GetQuestionAnswer("Would you like to overwrite it? (Y/N)"))
            {
                SaveFile.close();
                SaveFile.open(finalPath, std::ios::out | std::ios::trunc); // trunc: overwrite content
                for (int i = 0; i <= count; i++)
                    SaveFile << entries[i]
                             << " ";
                std::cout << "File saved successfully at:"
                          << std::endl
                          << finalPath
                          << std::endl
                          << "Press any key to continue..."
                          << std::endl;
            }
            else
                std::cout << "Save process cancelled!"
                          << std::endl;
        }
        else
        {
            // File does not exist
            SaveFile.open(finalPath, std::ios::out);
            for (int i = 0; i < count; i++)
                SaveFile << entries[i]
                         << " ";
            std::cout << "File saved successfully at:"
                      << std::endl
                      << finalPath
                      << std::endl
                      << "Press any key to continue..."
                      << std::endl;
            getch();
        }
    } while (!GetQuestionAnswer("Go back to the menu? (Y/N)"));
}

void GenericGraph::SetCustomSaveLocation()
{
    ShowStringWithPadding("Save to custom location", 26, '-');
    std::cout << "Enter the custom location of the file:"
              << std::endl;
    this->customSavePath = GetCustomPathFromConsole();
    this->useCustomSaveLocation = true;
    std::cout << "Custom Save Path saved successfully! You should proceed saving the file now."
              << std::endl
              << "Press any key to continue..."
              << std::endl;
    getch();
}

void GenericGraph::RestoreDefaultSaveLocation()
{
    this->useCustomSaveLocation = false;
    std::cout << "Default Save Path restored successfully! You should proceed saving the file now."
              << std::endl
              << "Press any key to continue..."
              << std::endl;
    getch();
}

bool CheckFormatOfDataSet(int *dataArr, int dataLen)
{
    // Instantiate a cursor to go through the dataArr and a temporary int to read data
    int cursor = 0,
        temp;
    // Get the no. nodes, increment counter
    int nodesCount = dataArr[cursor++];

    // The next entry is the source, increment counter
    temp = dataArr[cursor++];

    // Check if the src is part of the graph
    if (temp >= nodesCount || temp < 0)
        // The source is not part of the graph I.E. Bad format
        return false;

    // The next entry is the edgesCount, increment counter
    int edgesCount = dataArr[cursor++];

    // Check if the graph can sustain this many edges
    int maxEdges = nodesCount * (nodesCount - 1) / 2;
    if (edgesCount > maxEdges)
        // The count is larger than the number of edges the graph can have
        return false;

    // The remaining data consists in pairs of edge start-finish while (cursor < dataLen)
    {
        int start = dataArr[cursor++],
            finish = dataArr[cursor++];

        // Check if the edge's ends are part of the graph
        if (start < 0 || start >= nodesCount ||
            finish < 0 || finish >= nodesCount)
            // One of the edge's end are out of the graph
            return false;
    }

    // All tests passed
    return true;
}

char *GetFileNameFromConsole()
{
    std::cout << "Enter the name of the file(not including the extension):"
              << std::endl;
    return GetCustomPathFromConsole();
}

char *GetCustomPathFromConsole()
{
    // Instantiate a larger char array
    char *customPath = new char[256];
    // Flush the cin buffer before reading the path
    std::cin.clear();
    std::cin.sync();
    // Read the path
    std::cin.getline(customPath, 256);
    return customPath;
}
