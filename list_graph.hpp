#pragma once

#include "graph.hpp"

//represents node of graph as an element of doubly linked list
struct	Node {

private:

	// number of node; weight of edge that is required to get to this node from node on the other side of edge
	int m_number, m_weight; 

public:
	std::weak_ptr<Node>  pm_previous; //pointers to 
	std::shared_ptr<Node> pm_next; 	  //adjacent nodes

	const int& getNumber() const { return m_number; }; 
	const int& getWeight() const { return m_weight; };

	explicit Node(int t_number, int t_weight, std::weak_ptr<Node> pt_previous, std::shared_ptr<Node> pt_next)
		:m_number(t_number), m_weight(t_weight), pm_previous(pt_previous), pm_next(pt_next) {}
};


//represents element that always points to the end and beginning of doubly linked list
struct ListGuard {

	std::shared_ptr<Node> pm_head; //pointer to beginning 
	std::shared_ptr<Node> pm_tail; //pointer to end 
};


//graph in an adjency list representation 
class ListGraph : public Graph {

	std::shared_ptr<ListGuard[]> prgm_guards; //dynamically allocated array of guards							

public:
	void fillGraph(bool allowLoops) const override; 
	void printGraph() const override;
	int readFromFile() override;
	void createInput(int t_startNode) const override;

	void addNodeEnd(int t_number, int t_weight, int t_guardNumber) const; //adds node at the end of list
	bool detectIfExist(int t_guardNumber, int t_nodeNumber) const; //detects if given node exists
	std::shared_ptr<Node> getHeadOfGuard(int index) const; //returns node that head pointer of given list guard 
															//is pointing to
	explicit ListGraph(int t_V, double t_density);
	ListGraph() : Graph() {};
};
