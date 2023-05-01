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
    std::cout << stringWithPadding("Reading graph data", 18, '-')
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
                  << "]: ";
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

/*
void GenericGraph::Solve()
{
    int k = 0;
    int knownNodesIndex = 1,
        unknownNodesIndex = this->nodes->length() - 1,
        analyzedNodesIndex = 0,
        orderIndex = 0;

    // knownNodes of len 1 (only the src)
    // unknownNodes of nodes length because we insert all nodes, after which we remove the src
    // analyzed of length 0
    Array *knownNodes = new Array(1),
          *unknownNodes = new Array(this->nodes->length()),
          *analyzedNodes = new Array(),
          *order = new Array(this->nodes->length()),
          *position = new Array(this->nodes->length());

    // Initialize the order for the source
    order->set(this->src, ++k);

    // Starting from the source
    knownNodes->set(0, this->src);

    // Copy all nodes and remove the source
    for (int i = 0; i < this->nodes->length(); i++)
        unknownNodes->set(i, this->nodes->getValue(i));
    unknownNodes->removeByValue(this->src);

    if (this->log->getDebug())
    {
        std::cout << std::endl
                  << "Adjacency matrix: "
                  << std::endl;
        for (int i = 0; i < this->nodes->length(); i++)
        {
            for (int j = 0; j < this->nodes->length(); j++)
                std::cout << this->edges[i][j]
                          << ' ';
            std::cout << std::endl;
        }
    }

    while (*analyzedNodes != *this->nodes)
    {
        while (knownNodes->length() > 0)
        {
            // Select a node from knownNodes
            int x = knownNodes->first(),
                y;
            bool exists = false;
            for (y = 0; y < this->nodes->length(); y++)
                // If there is an edge from the selected node to an unknown node
                // stop the loop and keep said edge
                if (this->edges[x][y] == 1 && unknownNodes->getIndex(y) != -1)
                {
                    exists = true;
                    break;
                }

            if (exists)
            {
                // if it exists, move it from unknown to known
                unknownNodes->removeByValue(y);
                knownNodes->add(y);
                position->set(y, x);
                order->set(y, ++k);
            }
            else
            {
                // if there is no such edge, set the main point as completly analyzed
                knownNodes->removeByValue(x);
                analyzedNodes->add(x);
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

        // sort these before comparing them in the while
        analyzedNodes->sort();
        this->nodes->sort();
    }

    std::cout << "Algorithm finished.";
}
*/

void GenericGraph::Solve(){
    //BFS
}
#pragma endregion
