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
				std::cout << (iLoops * 100 / loops) + 1 << "%" << "\r";
			}
			file << sum / loops << " ";
			std::cout << density << " DENSITY DONE!\n";
		}
		std::cout << "\n" << vertices << " VERTICES DONE!\n\n";
	}
	file << "\n";

	return file;
}


int main() {

	srand((unsigned int)time(NULL));

	bool testZone = true; // YOUR CHOICE OF ZONE


	// TEST ZONE //
	if (testZone) {

		int vertTab[5] = { 1000, 20, 30, 40, 50};
		double densTab[4] = { 0.25, 0.5, 0.75, 1 };
		int loops = 100;

		std::ofstream file("OutputTimes.txt");


		tests<ListGraph>(file, vertTab, densTab, loops);
		std::cout << "LIST DONE\n";
		tests<MatrixGraph>(file, vertTab, densTab, loops);
		std::cout << "MATRIX DONE\n";


		file.close();

		return 0;
	}


	// FUN ZONE //
	typedef MatrixGraph ActualGraph; 
	bool useFile = true; 
	bool allowLoops = true;
	int vertices = 4;
	double density = 0.5;
	int startNode = 1;

	std::shared_ptr<ActualGraph> graph;

	if (useFile) {
		std::shared_ptr<ActualGraph> tmp = std::make_shared<ActualGraph>();
		graph = tmp;
		startNode = tmp->readFromFile();
	}
	else {
		std::shared_ptr<ActualGraph> tmp = std::make_shared<ActualGraph>(vertices, density);
		graph = tmp;
		tmp->fillGraph(allowLoops);
	}
	graph->printGraph();
	if(!useFile) graph->createInput(startNode);
	bellmanFord(std::move(graph), startNode, true);
	
	return 0;
}