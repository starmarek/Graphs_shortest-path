#include <time.h>

#include "bellman_algorithms.hpp"


template<typename T>
std::ostream& tests(std::ostream& file, int (&vertTab)[5], double (&densTab)[4], int loops) {
	
	for (int vertices : vertTab) {
		for (double density : densTab) {
			double sum = 0;
			for (int iLoops = 0; iLoops < loops; ++iLoops) {

				std::shared_ptr<T> graph = std::make_shared<T>(vertices, density);
				int startNode = rand() % vertices;

				graph->fillGraph(true);

				sum += bellmanFord(std::move(graph), startNode, false);
				std::cout << (iLoops * 100 / loops) + 1 << "%" << "\r" << std::flush;
			}
			file << sum / loops << " ";
			std::cout << density << " DENSITY DONE!" << std::endl;
		}
		std::cout << "\n" << vertices << " VERTICES DONE!\n" << std::endl;
	}
	file << "\n";

	return file;
}


int main() {

	srand(static_cast<unsigned int>(time(NULL)));

	bool testZone = true; // YOUR CHOICE OF ZONE
	

	// TEST ZONE //
	if (testZone) {

		int vertTab[5] = { 10, 20, 30, 40, 50 };
		double densTab[4] = { 0.25, 0.5, 0.75, 1 };
		int loops = 50;

		std::ofstream file("OutputTimes.txt");
		if (!file.is_open()) {

			std::cerr << "OUTPUT TIMES NOT OPEN!" << std::flush;
			return 1;
		}

		tests<ListGraph>(file, vertTab, densTab, loops);
		std::cout << "LIST DONE\n" << std::endl;
		tests<MatrixGraph>(file, vertTab, densTab, loops);
		std::cout << "MATRIX DONE" << std::endl;

		file.close();

		return 0;
	}


	// FUN ZONE //
	typedef MatrixGraph ActualGraph;
	bool useFile = true;
	bool allowLoops = false;
	int vertices = 5;
	double density = 1;
	int startNode = 2;

	std::shared_ptr<ActualGraph> graph;

	if (useFile) {
		std::shared_ptr<ActualGraph> tmp = std::make_shared<ActualGraph>();
		graph = tmp;
		try {
			startNode = tmp->readFromFile();
		}
		catch (const char* msg) {
			std::cerr << msg << std::flush;
			return 1;
		}
	}
	else {
		std::shared_ptr<ActualGraph> tmp = std::make_shared<ActualGraph>(vertices, density);
		graph = tmp;
		tmp->fillGraph(allowLoops);
	}

	graph->printGraph();
	try {
		if (!useFile) graph->createInput(startNode);
	}
	catch(const char* msg) {
		std::cerr << msg << std::flush;
		return 1;
	}
	bellmanFord(std::move(graph), startNode, true);

	return 0;
}