#pragma once

#include "graph.hpp"

//represents weighted edge between two nodes
struct Edge {
	
	//source node of edge; destination node of edge; weight of edge
	int source, dest, weight; 
};


//graph in an adjency list representation 
class ListGraph : public Graph {

	Edge* edge; //array of all adges in the graph

public:
	//those methods have comments in the Graph class
	void fillGraph(const bool allowLoops) const override; 
	void printGraph() const override;
	const int readFromFile() override;
	void createInput(const int t_startNode) const override;

	const Edge* getStruct() const { return edge; } 
	void addEdge(int t_guardNumber, int t_number, int t_weight, int flag) const; //adds node at the flag position
	bool detectIfExist(int t_guardNumber, int t_nodeNumber) const; //detects wheter given connection exists
	
	explicit ListGraph(int t_V, double t_density)
		:Graph(t_V, static_cast<int>(t_density* t_V* (t_V - 1)), t_density),
		 edge(new Edge[static_cast<int>(t_density* t_V* (t_V - 1))]) {}
	ListGraph() : Graph() {};
	~ListGraph() { delete[] edge; }
};
