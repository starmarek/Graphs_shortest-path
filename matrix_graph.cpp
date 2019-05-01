#include "matrix_graph.hpp"

//constructor
MatrixGraph::MatrixGraph(int t_V, double t_density) 
	: Graph(t_V, static_cast<int>(t_density)* t_V* (t_V - 1), t_density), //t_E official formula again (look->listgraph constructor)
	  matrix(std::move(std::make_unique<std::unique_ptr<int[]>[]>(t_V))) {

	for (int iCell = 0; iCell < m_V; ++iCell) {
		matrix[iCell] = std::move(std::make_unique<int[]>(t_V)); //initialzation of all arrays inside the main matrix array
	}
}


void MatrixGraph::fillGraph(bool allowLoops) const {

	if (m_density == 1) {  //full graph case
		for (int iRows = 0; iRows < m_V; ++iRows) { //for each row
			for (int jColumns = iRows + 1; jColumns < m_V; ++jColumns) { //with every iteration number of filled cells is 
				int foo_weight = rand() % 20 - 2;
				while (foo_weight == 0) {
					foo_weight = rand() % 20 - 2;
				}
				matrix[iRows][jColumns] = foo_weight;				//getting smaller -> rules of full graph
			}
		}
	}
	else { //other densities

		int foo_edges = m_E;

		while (foo_edges) { //as loong as will create appropriete number of edges

			int rowNode = rand() % m_V; //randomly choose numbers of nodes between which the edge will be created
			int columnNode = rand() % m_V;

			if (!matrix[rowNode][columnNode]) { //if the edge won't exist yet

				int foo_weight = rand() % 20 - 2;  //random weight of edge
				while (foo_weight == 0) {
					foo_weight = rand() % 20 - 2;
				}

				if (rowNode != columnNode) { //those if's detect if program want to crete a loop-edge to the same node
												//if user specified allowLopps = true, it will be created, if false,
					matrix[rowNode][columnNode] = foo_weight;		//iteration will be skipped and won't create any edge
					--foo_edges;
				}
				else if (allowLoops) {
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

	for (int iIter = 0; iIter < m_V; ++iIter) {
		
		if (iIter <= 10) std::cout << "    " << iIter;
		else if (iIter <= 100) std::cout << "   " << iIter;
		else std::cout << "  " << iIter;
	}
	std::cout << "\n\n";

	for (int iRows = 0; iRows < m_V; ++iRows) {	
		
		if (iRows < 10) std::cout << iRows << "   |";
		else if (iRows < 100) std::cout << iRows << "  |";
		else std::cout << iRows << " |";
		
		for (int jColumns = 0; jColumns < m_V; ++jColumns) {

			int foo = matrix[iRows][jColumns];

			if (foo == 0) std::cout << "*";
			else std::cout << foo;

			if (abs(foo) < 10) std::cout << "    ";
			else if (abs(foo) < 100) std::cout << "   ";
			else std::cout << "  ";

			if (foo < 0) std::cout << '\b';
		}
		std::cout << "|\n";
	}
	std::cout << std::endl;
}


//method also takes care of object initialization
int MatrixGraph::readFromFile() {

	int start, source, destination, weight;

	std::ifstream file("Input.txt"); 
	if (!file.is_open()) {
		throw "INPUT MATRIX NOT OPEN!";
		return 1;
	}

	//attributes and matrix array initialization; start - starting node (not attribute)
	file >> m_E >> m_V >> start;
	matrix = std::move(std::make_unique<std::unique_ptr<int[]>[]>(m_V));

	//nested arrays initialization
	for (int iCell = 0; iCell < m_V; ++iCell) {
		matrix[iCell] = std::move(std::make_unique<int[]>(m_V));
	}

	//filling the arrays
	for (int iEdge = 0; iEdge < m_E; ++iEdge) {

		file >> source >> destination >> weight;

		matrix[source][destination] = weight;;
	}
	file.close();

	return start; //returns the starting node (bellmanFord algorithm required information)
}


void MatrixGraph::createInput(int t_startNode) const {

	std::ofstream file("CreatedInput.txt");
	if (!file.is_open()) {
		throw  "CREATED INPUT MATRIX NOT OPEN!";
		return;
	}

	file << m_E << " " << m_V << " " << t_startNode << "\n";
	for (int iRow = 0; iRow < m_V; ++iRow) {
		for (int iColumn = 0; iColumn < m_V; ++iColumn) {

			if (matrix[iRow][iColumn] != 0) {

				file << iRow << " ";
				file << iColumn << " ";
				file << matrix[iRow][iColumn] << "\n";
			}
		}
	}
	file.close();
}
