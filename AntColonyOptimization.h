#include <string>
#include <list>
#include <array>
#include <vector> 
#include <time.h>
#include <stdlib.h>
//#pragma once
using namespace std;



void show(float*** pointer, int row, int col, int depth) {
	for (int k = 0; k < depth; k++) {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++)
				cout << pointer[i][j][k] << " ";;
			cout << endl;
		}
		cout << endl<< endl;
	}
}
void show(float** pointer, int row, int col) {

	for (int j = 0; j < row; j++) {
	for (int i = 0; i < col; i++) 
			cout << pointer[i][j] << " ";
		cout << endl;
	}
}
void show(bool** pointer, int row, int col) {

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++)
			cout << (pointer[i][j] ? "True":"False") << " ";
		cout << endl;
	}
}
void show(float* pointer, int len) {

	for (int i = 0; i < len; i++)
		cout << pointer[i] << " ";;
}
void show(int* pointer, int len) {

	for (int i = 0; i < len; i++)
		cout << pointer[i] << " ";;
}

class Ant
{
public:
	Graph* G;
	list<int*> path;
	list<int*> nodesColor;
	list<int> Nodes;
	float* nodesOrder;
	int Cost;
	Ant(Graph*);
	~Ant();
	void fillPossiblePath(float**, int);
	float* existedNodesOrder();
	int getColor(int);
	float calcCost();
	static Ant* do_optimization(Graph*, int, int, float);
	static int* rouletteWheelSelection(float**,int, int);
	static int rouletteWheelSelection(float*, int);
	static int random(int);
	static float random();
private:

};

Ant::Ant(Graph *G)
{
	Ant::G = G;
	nodesOrder = new float[G->num_nodes];
	for (int i = 0; i < G->num_nodes; i++) {
		int counter = 0;
		for (int j = 0; j < G->num_nodes; j++)
			if (G->CM[i][j])
				counter++;
		nodesOrder[i] = counter;
	}
}

Ant::~Ant()
{
	delete G;
	delete nodesOrder;
}
void Ant::fillPossiblePath(float** possible_path, int lnode) {
	possible_path[lnode][1] = 0;
	possible_path[lnode][0] = 0;
	for (auto nc : nodesColor)
		possible_path[*nc][1] = 0;
	//show(possible_path, 2, G->num_nodes);
	for (int i = 0; i < G->num_nodes; i++)
	{
		if (possible_path[i][0] == 0)
			continue;
		int colori = getColor(i);
		if (colori != -1) {
			for (int j = 0; j < G->num_nodes; j++)
				if (G->CM[lnode][j])
					if (colori == getColor(j))
						possible_path[i][0] = 0;
		}else
			possible_path[i][0] = 0;
	}
	//show(possible_path, 2, G->num_nodes);
}
float* Ant::existedNodesOrder() {
	static float* tmp = new float[G->num_nodes];
	for (int u = 0; u < G->num_nodes; u++) {
		tmp[u] = nodesOrder[u];
		for (auto nc : nodesColor)
			if (*nc == u) {
				tmp[u] = 0;
				break;
			}
	}
	return tmp;
}
int Ant::getColor(int node) {
	for (auto nc : nodesColor)
		if (*nc == node)
			return *(nc + 1);
	return -1;
}
float Ant::calcCost() {
	for (auto it1 : nodesColor)
		for (auto it2 : nodesColor)
		{
			if (*(it1) < *(it2)) {
				int tmp;
				tmp = *(it1);
				*(it1) = *(it2);
				*(it2) = tmp;
				tmp = *(it1 + 1);
				*(it1 + 1) = *(it2 + 1);
				*(it2 + 1) = tmp;
			}
		}
	float c = -1;
	for (auto nc : nodesColor)
		if (*(nc + 1) > c)
			c = *(nc + 1);
	Cost = c + 1;
	return c + 1;
}
Ant* Ant::do_optimization(Graph *G, int num_ants, int Iter, float rho) {
	int num_nodes = G->num_nodes;
	float*** pheromone;
	float** possible_pass;
	pheromone = new float**[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		pheromone[i] = new float*[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			pheromone[i][j] = new float[2];
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			for (int k = 0; k < 2; k++)
				pheromone[i][j][k] = 1;
	for (int i = 0; i < num_nodes; i++)
		for (int j = 0; j < num_nodes; j++)
			if (G->CM[i][j])
				pheromone[i][j][0] = 0;
	//show(G->CM, num_nodes, num_nodes);
	//show(pheromone, num_nodes, num_nodes, 2);
	possible_pass = new float*[num_nodes];
	for (int i = 0; i < num_nodes; i++)
		possible_pass[i] = new float[2];
	int rowCol[2]{0,0};
	int* tmp1;
	int* tmp2;
	float* eno;
	vector<Ant*> ants;
	static Ant* bestAnt;
	float bestAntCost = num_ants + 1;
	for (int iter = 0; iter < Iter; iter++)
	{
		ants.clear();
		for (int i = 0; i < num_ants; i++)
			ants.push_back(new Ant(G));
		for (int aind = 0; aind < num_ants; aind++)
		{
			int cCounter = 0;
			int lnode = rouletteWheelSelection(ants[aind]->nodesOrder, num_nodes);

			for (int k = 0; k < num_nodes; k++)
			{
				for (int i = 0; i < num_nodes; i++)
					for (int j = 0; j < 2; j++)
						possible_pass[i][j] = pheromone[lnode][i][j];
				//show(possible_pass, 2, num_nodes);
				ants[aind]->fillPossiblePath(possible_pass, lnode);
				//show(possible_pass, 2, num_nodes);
				int* tmp = rouletteWheelSelection(possible_pass, 2, num_nodes);

				if (tmp == NULL)
				{
					*(rowCol) = 1;
					*(rowCol + 1) = lnode;
				}
				else
				{
					*(rowCol) = *(tmp);
					*(rowCol + 1) = *(tmp + 1);
				}
				if (*rowCol == 1)
				{
					tmp1 = new int[2]{ lnode ,cCounter };
					tmp2 = new int[4]{ lnode,*rowCol,*(rowCol + 1) ,cCounter };
					ants[aind]->nodesColor.push_back(tmp1);
					ants[aind]->path.push_back(tmp2);
					lnode = *(rowCol + 1);
					cCounter++;
				}
				else
				{
					tmp1 = new int[2]{ lnode ,ants[aind]->getColor(*(rowCol + 1)) };
					tmp2 = new int[4]{ lnode,*rowCol,*(rowCol + 1) ,ants[aind]->getColor(*(rowCol + 1)) };
					ants[aind]->nodesColor.push_back(tmp1);
					ants[aind]->path.push_back(tmp2);
					lnode = *(rowCol + 1);
					eno = ants[aind]->existedNodesOrder();
					lnode = rouletteWheelSelection(eno, num_nodes);
				}
			}
			ants[aind]->calcCost();
			if (ants[aind]->Cost <= bestAntCost) {
				bestAnt = ants[aind];
				bestAntCost = ants[aind]->Cost;
			}
		}
		for (auto pth : bestAnt->path) {
			pheromone[pth[0]][pth[2]][pth[1]] += (1 / pow(bestAnt->Cost, 0.3));
	/*		show(pth, 4);
			cout << endl;*/
		}
		for (int i = 0; i < num_nodes; i++)
			for (int j = 0; j < num_nodes; j++)
				for (int k = 0; k < 2; k++)
					pheromone[i][j][k] *= (1-rho);
	}

	//show(pheromone, num_nodes, num_nodes, 2);
	delete[] pheromone;
	delete[] possible_pass;
	return bestAnt;
}
int* Ant::rouletteWheelSelection(float** P, int row, int col) {
	int len = row*col;
	float* p = new float[len];
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++)
			p[j*col + i] = P[i][j];
	}
	//show(p, len);
	for (int i = 1; i < len; i++)
		p[i] += p[i - 1];
	if (p[len - 1] == 0)
		return nullptr;
	for (int i = 0; i < len; i++)
		p[i] /= p[len - 1];
	//show(p, len);
	int ind = 0;
	int rowCol[2];
	float e = random();
	for (; ind < len; ind++)
		if (e <= p[ind])
			break;
	delete p;
	if (ind == len) {
		rowCol[0] = row - 1;
		rowCol[1] = col - 1;
	}
	else {
		rowCol[1] = ind%col;
		rowCol[0] = (int)(ind / col);
	}
	return rowCol;
}
int Ant::rouletteWheelSelection(float* P, int len) {
	float* p = new float[len];
	for (int i = 0; i < len; i++)
		p[i] = P[i];

	for (int i = 1; i < len; i++)
		p[i] += p[i - 1];
	for (int i = 0; i < len; i++)
		p[i] /= p[len - 1];
	int ind = 0;
	int sInd = 0;
	float e = random();
	for (; sInd < len; sInd++)
		if (e <= p[sInd])
			break;
	delete p;
	if (sInd == len)
		return len - 1;
	return sInd;
}
int Ant::random(int length) {
	srand((unsigned)(time(NULL) * 1000));
	return  rand() % length;
}
float Ant::random() {
	srand((unsigned)(time(NULL) * 1000));
	return rand() / (RAND_MAX + 1.);
}


