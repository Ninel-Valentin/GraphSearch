#include <iostream>

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
    int src = 0;
    Array *nodes;
    int **edges;

public:
    GenericGraph();
    ~GenericGraph();
    void InitializeGraph(loggingSystem *);
    void Solve(Array *, Array *);
};

GenericGraph::GenericGraph()
{
    this->nodes = new Array;
}
GenericGraph::~GenericGraph()
{
    if (this->nodes->length() > 0)
    {
        for (int i = 0; i < this->nodes->length(); i++)
            delete[] edges[i];
        delete[] edges;
        delete nodes;
    }
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

// This function provides the position array as the first param, and the length array as the second param
void GenericGraph::Solve(Array *positionArr, Array *lengthArr)
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

    delete[] lengthArr;
    lengthArr = new Array(this->nodes->length());
    delete[] positionArr;
    positionArr = new Array(this->nodes->length()); // All values initialized with 0

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
                    positionArr->set(y, x);
                    lengthArr->set(y, lengthArr->getValue(x) + 1);
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

    std::cout << "Finished!";
}
