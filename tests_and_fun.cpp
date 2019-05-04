#include <time.h>

#include "bellman_algorithms.hpp"


//function that conducts test
//it is templated with representation of graph. Arguments that are passed:
//@loops - how many times the algorithm will solve objects with the same parameters(the more the more accurate outcome)
//@verTab,@densTab - parameters of objects that will be created 
template<typename T>
std::ostream& tests(std::ostream& file, int (&vertTab)[5], double (&densTab)[4], int loops) {
	
	for (double density : densTab) { //for each density
		for (int vertices : vertTab) { //for each amount of vertices
			double sum = 0;
			for (int iLoops = 0; iLoops < loops; ++iLoops) { //as many times as loops variable was set

				std::shared_ptr<T> graph = std::make_shared<T>(vertices, density); //creating appropriate object
				int startNode = rand() % vertices; //generating rand starting node

				graph->fillGraph(true); //fill graph with loops possibility

				sum += bellmanFord(std::move(graph), startNode, false); //bellman returning time of execution
				std::cout << (iLoops * 100 / loops) + 1 << "%" << "\r" << std::flush;
			}
			file << sum / loops << " "; //saving the average
			std::cout << vertices << " VERTICES DONE!" << std::endl;
		}
		std::cout << "\n" << density << " DENISTY DONE!\n" << std::endl;
	}
	file << "\n";
	return file;
}


int main() {

	srand(static_cast<unsigned int>(time(NULL)));

	bool testZone = true; // YOUR CHOICE OF ZONE
	

	// TEST ZONE //

	//conduct automatic testing for graphs that suits you!

	if (testZone) {
		
		/////////////////////////
		// changeable variables//
		int vertTab[5] = {5, 15, 30, 50, 100}; //amounts of vertices to be tested
		double densTab[4] = {0.25, 0.5, 0.75, 1}; //densities to be tested
		int loops = 100;		// how many times the algorithm will solve objects with the same parameters
		/////////////////////////

		std::ofstream file("OutputTimes.txt");
		if (!file.is_open()) {

			std::cerr << "OUTPUT TIMES NOT OPEN!" << std::flush;
			return 1;
		}

		tests<ListGraph>(file, vertTab, densTab, loops); //testing for List
		std::cout << "LIST DONE\n" << std::endl;
		tests<MatrixGraph>(file, vertTab, densTab, loops);	//testing for Matrix
		std::cout << "MATRIX DONE" << std::endl;

		file.close();
		return 0;
	}


	// FUN ZONE //
	
	//create your own graph!

	/////////////////////////
	// changeable variables//
	typedef	ListGraph ActualGraph; //graph representation
	bool useFile = false;	//input from file (Input.txt)
	bool allowLoops = true;	//if loops from node to the same node should be created
	int vertices = 4;		//amount of vertices
	double density = 0.5;	//density of graph
	int startNode = 2;		//starting node for bellmanFord
	/////////////////////////

	std::shared_ptr<ActualGraph> graph;

	if (useFile) { //if input from file
		std::shared_ptr<ActualGraph> tmp = std::make_shared<ActualGraph>();
		graph = tmp;
		try {
			startNode = tmp->readFromFile(); //if inputed correctly there will be no exception
		}
		catch (const char* msg) {
			std::cerr << msg << std::flush;
			return 1;
		}
	}
	else { //if random graph
		std::shared_ptr<ActualGraph> tmp = std::make_shared<ActualGraph>(vertices, density);
		graph = tmp;
		tmp->fillGraph(allowLoops); //random fill
	}

	graph->printGraph(); 
	try {
		if (!useFile) graph->createInput(startNode); //if created succesfully there will be no exception
	}
	catch(const char* msg) {
		std::cerr << msg << std::flush;
		return 1;
	}
	bellmanFord(std::move(graph), startNode, true); //solving with bellman ford

	return 0;
}