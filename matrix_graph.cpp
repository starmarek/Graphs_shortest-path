#include "matrix_graph.hpp"

static int s_infinity = 1000000; //should be the same as in bellmanAlogrithms.cpp

static int s_balance_of_interval = -1; //balance for rand() in the fill method - interval is set
										//to 20 digits (0 - 19) originally. Using this variable you 
										//can manipulate it. For example changing to (-1) interval will be set to 
										//(-1 - 18)

//constructor
MatrixGraph::MatrixGraph(int t_V, double t_density) 
	: Graph(t_V, static_cast<int>(t_density* t_V* (t_V - 1)), t_density), //t_E official formula again (look->listgraph constructor)
	  matrix(std::make_unique<std::unique_ptr<int[]>[]>(t_V)) {

	//initialzation of all arrays inside the main matrix array
	for (int iCell = 0; iCell < m_V; ++iCell) {

		matrix[iCell] = std::make_unique<int[]>(t_V); 
		for (int iColumn = 0; iColumn < m_V; ++iColumn) {
			
			if (iCell == iColumn) matrix[iCell][iColumn] = 0; //diagonal to zeros
			else matrix[iCell][iColumn] = s_infinity; //every other cell to "infinity"
		}
	}
}


void MatrixGraph::fillGraph(const bool t_allowLoops) const {

	if (m_density == 1) {  //full graph case
		for (int iRows = 0; iRows < m_V; ++iRows) { //for each row
			for (int jColumns = 0; jColumns < m_V; ++jColumns) {  //for each column
				if (iRows != jColumns) //don't create a loop to the same node
				{
					int foo_weight = rand() % 20  + s_balance_of_interval;
					while (foo_weight == 0) { //draw weight other than zero

						foo_weight = rand() % 20 + s_balance_of_interval;
					}
					matrix[iRows][jColumns] = foo_weight;
				}
			}
		}
	}
	else { //other densities

		int foo_edges = m_E;

		while (foo_edges) { //as loong as will create appropriete number of edges

			int rowNode = rand() % m_V; //randomly choose numbers of nodes between which the edge will be created
			int columnNode = rand() % m_V;

			//if the edge won't exist yet which means, if it is still set to 0 or infinity
			if (matrix[rowNode][columnNode] == 0|| matrix[rowNode][columnNode]== s_infinity) { 

				int foo_weight = rand() % 20 + s_balance_of_interval;
				while (foo_weight == 0) { //draw random other than 0

					foo_weight = rand() % 20 + s_balance_of_interval;
				}

				//following (if's) detect if program want to crete a loop-edge to the same node
				//if user specified allowLopps = true, it will be created, if false,
				//iteration will be skipped and won't create any edge
				if (rowNode != columnNode) {

					matrix[rowNode][columnNode] = foo_weight;
					--foo_edges;
				}
				else if (t_allowLoops) {

					matrix[rowNode][columnNode] = foo_weight;
					--foo_edges;
				}
			}
		}
	}
}


//method changes its behavior depending on length of digits; its also able to detect minus ( - ) symbol
void MatrixGraph::printGraph() const {

	std::cout << std::string(33, '-') << "\n  AdjencyMatrix Representation\n\n ";

	//printing digits on the top
	for (int iIter = 0; iIter < m_V; ++iIter) {
		
		if (iIter <= 10) std::cout << "    " << iIter;
		else if (iIter <= 100) std::cout << "   " << iIter;
		else std::cout << "  " << iIter;
	}
	std::cout << "\n\n";

	//printing all other rows
	for (int iRows = 0; iRows < m_V; ++iRows) {	
		
		//first "wall" of matix
		if (iRows < 10) std::cout << iRows << "   |";
		else if (iRows < 100) std::cout << iRows << "  |";
		else std::cout << iRows << " |";
		
		//all connections in actual row
		for (int jColumns = 0; jColumns < m_V; ++jColumns) {

			int foo = matrix[iRows][jColumns];

			if (foo == s_infinity) std::cout << "*"; // if node is set to "infinity" then print a star 
			else std::cout << foo;					// as a symbol of no connection

			if (abs(foo) < 10 || abs(foo) == s_infinity) std::cout << "    "; //1000000 counts as a star so single digit space
			else if (abs(foo) < 100) std::cout << "   ";
			else std::cout << "  ";

			if (foo < 0) std::cout << '\b'; //because there was a minus sign return carriege 
		}
		//second "wall"
		std::cout << "|\n";
	}
	std::cout << std::endl;
}


//method also takes care of object initialization
const int MatrixGraph::readFromFile() {

	std::ifstream file("Input.txt"); 
	if (!file.is_open()) {
		throw "INPUT MATRIX NOT OPEN!";
		return 1;
	}

	//attributes and matrix array initialization; start -> starting node (not attribute)
	int start, source, destination, weight;
	file >> m_E >> m_V >> start;
	matrix = std::make_unique<std::unique_ptr<int[]>[]>(m_V);

	//nested arrays initialization (the same as in constructor)
	for (int iRow = 0; iRow < m_V; ++iRow) {

		matrix[iRow] = std::move(std::make_unique<int[]>(m_V));
		for (int iColumn = 0; iColumn < m_V; ++iColumn) {

			if (iRow == iColumn) matrix[iRow][iColumn] = 0;
			else matrix[iRow][iColumn] = s_infinity;
		}
	}

	//filling the arrays
	for (int iEdge = 0; iEdge < m_E; ++iEdge) {

		file >> source >> destination >> weight;
		matrix[source][destination] = weight;;
	}
	file.close();
	return start; //returns the starting node (bellmanFord's algorithm required information)
}


//saves information in the following order:
//
// amount of edges | amount of vertices | number of starting_node
//
// #edges following#
//
// number of source node | number of destination node | weight of edge to get to this node from source
//...
void MatrixGraph::createInput(const int t_startNode) const {
	
	std::ofstream file("CreatedInput.txt");
	if (!file.is_open()) {
		throw  "CREATED INPUT MATRIX NOT OPEN!";
		return;
	}

	file << m_E << " " << m_V << " " << t_startNode << "\n";
	for (int iRow = 0; iRow < m_V; ++iRow) {
		for (int iColumn = 0; iColumn < m_V; ++iColumn) {

			if (matrix[iRow][iColumn] != s_infinity && matrix[iRow][iColumn] != 0) {

				file << iRow << " ";
				file << iColumn << " ";
				file << matrix[iRow][iColumn] << "\n";
			}
		}
	}
	file.close();
}
