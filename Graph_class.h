#include <string>
#include <list>
//#pragma once
using namespace std;
class Graph
{
public:
	int num_nodes = 0;
	//Connection Matrix
	bool** CM;
	Graph(int);
	~Graph();
	void assign_nodes_a_number(string, int);
	void addEdge(string,string);
private:
	string* Nods_name;
	int get_node_num(string);
};

Graph::Graph(int num_nodes)
{
	this->num_nodes = num_nodes;
	CM = new bool*[num_nodes];
	for (int i = 0; i<num_nodes; i++)
		CM[i] = new bool[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			CM[i][j] = false;
	Nods_name = new string[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		Nods_name[i] = to_string(i);
}

Graph::~Graph()
{
	delete[] CM;
}
void Graph::assign_nodes_a_number(string name, int num) {
	Nods_name[num] = name;
}
void Graph::addEdge(string node1, string node2) {
	int node1_ind = get_node_num(node1);
	int node2_ind = get_node_num(node2);
	CM[node1_ind][node2_ind] = true;
	CM[node2_ind][node1_ind] = true;
}
int Graph::get_node_num(string node_name) {
	int i = 0;
	for (; i < num_nodes; i++)
		if (Nods_name[i] == node_name)
			break;
	return i;
}