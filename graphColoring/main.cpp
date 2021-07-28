#include <iostream>
#include <windows.h>
#include <list>
//#include <fstream>
#include <string>
#include "Graph_Class.h"
#include "..\greedy.h"
#include "AntColonyOptimization.h"
//#include <stdio.h>
//#include <stdlib.h>
using namespace std;
Graph* open_xml(string);
void interpret_xml(string, list<string>*, list<string>*);


int main()
{
	static Graph* graph = open_xml("graph.xml");
	cout << "algorithm_1 --> Greedy "<< endl <<
		"(code by:https://www.geeksforgeeks.org/graph-coloring-set-2-greedy-algorithm)" << endl;
	GreedyGraph GG(graph);
	GG.Start();
	cout <<endl << " Cost:" << GG.Cost << endl << endl;
	for (int i = 0; i < GG.V; i++)
		cout << "Vertex " << i << " ---> Color "
		<< GG.result[i] << endl;

	cout << endl << "algorithm_1 --> Ant Colony Optimization:";
	Ant* bestAnt = Ant::do_optimization(graph, graph->num_nodes, 50, 0.05);
	cout << endl << endl << " Cost:" << bestAnt->Cost << endl << endl;
	for (auto nc : bestAnt->nodesColor)
		cout << "Vertex " << *(nc) << " ---> Color "
		<< *(nc + 1) << endl;

	getchar();
	return 0;
}

Graph* open_xml(string file_name)
{
	FILE* fp = fopen(file_name.c_str(), "r");
	char st;
	string str = "";
	int num_node = 100;
	for (int i = 0; i < 1e7; ++i)
	{
		int stat = fread(&st, 1, 1, fp);
		if (stat == 0)
			break;
		str += st;
	}
	list<string> Nodes; list<string> Edges;
	interpret_xml(str, &Nodes, &Edges);
	static Graph graph(Nodes.size());
	int i = 0;
	for (string st : Nodes) {
		graph.assign_nodes_a_number(st, i);
		i++;
	}
	for (string st : Edges)
		graph.addEdge(st.substr(0, st.find(" ")), st.substr(st.find(" ") + 1, st.length()));
	return &graph;
}
void interpret_xml(string xml, list<string> *Nodes,  list<string> *Edges)
{
	const int num_keywords = 3;
	string keywords[num_keywords] = { "node name", "src", "dst" };
	int mem1 = 0,mem2 = 0;
	//for (int j = 0; j < num_keywords; j++)
		for (int i = 0; i < xml.length(); i++) {
			if (xml[i] == keywords[0][mem1]) {
				mem1++;
				if (mem1 == keywords[0].length())
				{
					string node_name = "";
					int k = i + 1;
					bool tmp1 = false;
					for (; xml[k] != '\n'; k++) {
						if (xml[k] == '"')
							tmp1 = !tmp1;
						if (tmp1) {
							if (xml[k + 1] == '"')
								break;
							if (node_name == "")
								node_name += xml[k + 1];
							else if (node_name != "")
								node_name += xml[k + 1];
						}
					}
					i = k;
					mem1 = 0;
					Nodes->push_back(node_name);
				}
			}
			else
			{
				mem1 = 0;
			}
			if (xml[i] == keywords[1][mem2]) {
				mem2++;
				if (mem2 == keywords[1].length())
				{
					string Edge = "";
					int k = i + 1;
					bool tmp1 = false, tmp2 = true;
					for (; xml[k] != '\n'; k++) {
						if (xml[k] == '"')
							tmp1 = !tmp1;
						if (tmp1) {
							if (xml[k + 1] == '"') {
								if (tmp2)
									Edge += " ";
								tmp2 = false;
								continue;
							}
							if (Edge == "")
								Edge += xml[k + 1];
							else if (Edge != "")
								Edge += xml[k + 1];
						}
					}
					i = k;
					mem2 = 0;
					Edges->push_back(Edge);
				}
			}
			else
			{
				mem2 = 0;
			}
		}
}