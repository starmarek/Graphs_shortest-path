#pragma once

#include "graph.hpp" 

class MatrixGraph : public Graph {

	std::unique_ptr<std::unique_ptr<int[]>[]> matrix; //dynamic array of dynamic arrays; main container of graph

public:
	void fillGraph(bool allowLoops) const override;
	void printGraph() const override;
	int readFromFile() override;
	void createInput(int t_startNode) const override;

	const int& getWeight(int t_row, int t_column) const { return matrix[t_row][t_column]; } 
	//returns value of given cell in matrix array

	explicit MatrixGraph(int t_V, double t_density);
	MatrixGraph() : Graph() {};
};