#pragma once

#include <stdlib.h>
#include <memory>
#include <iostream>
#include <fstream>


//base abstract class, representaing any kind of graph
class Graph {

protected:
	int m_V, m_E; //amount of Vertices and Edges
	double m_density; 

public:
	const int& getV() const { return m_V; }						
	const int& getE() const { return m_E; }
	virtual void fillGraph(bool allowLoops) const = 0; //fills graph in random way
	virtual void printGraph() const =  0; //prints graph representation into console
	virtual int readFromFile() = 0; //fills graph based on input file
	virtual void createInput(int t_startNode) const = 0; //creates artificial input file so user can copy it to
													//external graph-visualization tool
	virtual ~Graph() {};
	explicit Graph(int t_V, int t_E, double t_density) : m_V(t_V), m_E(t_E), m_density(t_density) {}
	Graph() {}; //Unparametrized constructor, so program can crete a graph without initialization. This is beacause program
};				//must be able to crete graph based on input file.