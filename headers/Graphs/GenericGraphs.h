#include <iostream>
#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H
#include "../loggingSystem.h"
#endif
#include "Array.h"

class GenericGraph
{
    int src = 0;
    Array *nodes;
    int **edges;
    loggingSystem *log;

public:
    GenericGraph();
    GenericGraph(loggingSystem *);
    ~GenericGraph();
    void InitializeGraph();
    void Solve();
};

#pragma region Constructors definitions
GenericGraph::GenericGraph()
{
}
GenericGraph::GenericGraph(loggingSystem *_log)
{
    this->nodes = new Array;
    log = _log;
}

GenericGraph::~GenericGraph()
{
    if (this->nodes->length() > 0)
        delete nodes;
    if (this->nodes->length() > 0)
    {
        for (int i = 0; i < this->nodes->length(); i++)
            delete[] edges[i];
        delete[] edges;
    }
}

void GenericGraph::InitializeGraph()
{
    if (this->log->getClearConsole())
        system("CLS");
    std::cout
        << std::string(paddingOfString(18), '-')
        << "Reading graph data"
        << std::string(paddingOfString(18), '-')
        << std::endl
        << "Enter the number of nodes in the graph: ";
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
                  << "]:"
                  << std::endl;
        std::cin >> this->src;
        // If the src is part of the current nodes, break the loop
        if (this->src <= this->nodes->length() - 1 &&
            this->src >= 0)
            break;
        std::cout << "Incorrect value! Retrying...";
    }

    int edgesCount = 0;
    while (true)
    {
        std::cout
            << "Please enter the number of edges in the graph: ";
        std::cin >> edgesCount;
        if (this->nodes->length() * (this->nodes->length() - 1) / 2 >= this->nodes->length())
            break;
        else
            std::cout << "You can not have more than "
                      << this->nodes->length() * (this->nodes->length() - 1) / 2
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
    int k = 0;
    int knownNodesIndex = 1,
        unknownNodesIndex = this->nodes->length() - 1,
        analyzedNodesIndex = 0,
        orderIndex = 0;

    Array *knownNodes = new Array(this->nodes->length()),
          *unknownNodes = new Array(this->nodes->length()),
          *analyzedNodes = new Array(this->nodes->length()),
          *order = new Array(this->nodes->length()),
          *position = new Array(this->nodes->length());

    // Starting from the source
    knownNodes->set(0, this->src);

    // Copy all nodes except the source
    for (int i = 0, addition = 0; i < this->nodes->length() - 1; i++)
    {
        // If we find the source, we just increment a temporary offset of the array to skip the source
        if (this->nodes[i] == this->src)
            addition++;
        unknownNodes->set(i, this->nodes->getValue(i + addition));
    }

    // Instantiate the position array
    for (int i = 0; i < this->nodes->length(); i++)
        position[i] = 0;

    while (unknownNodes != this->nodes)
    {
        while (knownNodes->length() > 0)
        {
            // Select a node from knownNodes
            int x = knownNodes->first();
            for (int y = 0; y < this->nodes->length(); y++)
            {
                // If there is an edge from the selected node to an unknown node
                std::cout << std::endl
                          << y;
                if (this->edges[x][y] == 1 && unknownNodes->getIndex(y) != -1)
                {
                    // Get the other edge end
                    unknownNodes->removeByValue(y);
                    knownNodes->add(y);
                    position[y] = x;
                    order[y] = ++k;
                }
                else
                {
                    knownNodes->removeByValue(x);
                    analyzedNodes->add(x);
                }
            }
        }
        // If there are still nodes not completly analyzed but we have no adjacent nodes, switch the source node
        if (unknownNodes->length() > 0)
        {
            int newSource = unknownNodes->first();
            knownNodes->add(newSource);
            unknownNodes->removeByValue(newSource);
            order[newSource] = ++k;
        }
    }

    std::cout << "Algorithm finished.";
}
#pragma endregion
