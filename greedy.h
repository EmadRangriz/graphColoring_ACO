// A C++ program to implement greedy algorithm for graph coloring
#define _GNU_SOURCE
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include "..\graphColoring\graphColoring\Graph_Class.h"
using namespace std;

// A class that represents an undirected graph
	class GreedyGraph
	{
	public:
		list<int> *adj; // A dynamic array of adjacency lists
		int V; // No. of vertices
		Graph* G;
		int* result;
		float Cost;
		// Constructor and destructor
		GreedyGraph(Graph*);
		~GreedyGraph() { delete[] adj; }

		// function to add an edge to graph
		void addEdge(int v, int w);
		Graph* Start();
		// Prints greedy coloring of the vertices
		void greedyColoring();
	};
	GreedyGraph::GreedyGraph(Graph *G) {
		this->V = G->num_nodes; adj = new list<int>[G->num_nodes];
		Cost = -1;
		GreedyGraph::G = G;
		for (int i = 0; i < G->num_nodes; i++)
			for (int j = i + 1; j < G->num_nodes; j++)
				if (G->CM[i][ j])
					addEdge(i, j);
	}
	void GreedyGraph::addEdge(int v, int w)
	{
		adj[v].push_back(w);
		adj[w].push_back(v); // Note: the graph is undirected
	}

	// Assigns colors (starting from 0) to all vertices and prints
	// the assignment of colors
	void GreedyGraph::greedyColoring()
	{
		result = new int[V];

		// Assign the first color to first vertex
		result[0] = 0;

		// Initialize remaining V-1 vertices as unassigned
		for (int u = 1; u < V; u++)
			result[u] = -1; // no color is assigned to u

		// A temporary array to store the available colors. True
		// value of available[cr] would mean that the color cr is
		// assigned to one of its adjacent vertices
		bool* available = new bool[V];
		for (int cr = 0; cr < V; cr++)
			available[cr] = false;

		// Assign colors to remaining V-1 vertices
		for (int u = 1; u < V; u++)
		{
			// Process all adjacent vertices and flag their colors
			// as unavailable
			list<int>::iterator i;
			for (i = adj[u].begin(); i != adj[u].end(); ++i)
				if (result[*i] != -1)
					available[result[*i]] = true;

			// Find the first available color
			int cr;
			for (cr = 0; cr < V; cr++)
				if (available[cr] == false)
					break;

			result[u] = cr; // Assign the found color

			// Reset the values back to false for the next iteration
			for (i = adj[u].begin(); i != adj[u].end(); ++i)
				if (result[*i] != -1)
					available[result[*i]] = false;
		}
		for (int cr = 0; cr < V; cr++)
			if (result[cr] > Cost)
				Cost = result[cr];
		Cost++;
	}

	// Driver program to test above function
	Graph* GreedyGraph::Start()
	{
		greedyColoring();
		return G;
	}
