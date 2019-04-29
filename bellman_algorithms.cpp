#include "bellman_algorithms.hpp"

static int  infinity = 1000000;
static int  neg_infinity = -1000000;


//prints to console and to just created output file the result of bellmanFord algorithm
void finalSolution(int t_dist[], int t_n, int t_startNode) {

	std::ofstream file("Output.txt");

	std::cout << std::string(20, '-') << "\n   FinalSolution\n\n";
	std::cout << "The starting node was -> " << t_startNode << "\n\n";

	file << std::string(20, '-') << "\n   FinalSolution\n\n";
	file << "The starting node was -> " << t_startNode << "\n\n";

	for (int i = 0; i < t_n; ++i) {
		if (t_dist[i] == neg_infinity) {
			std::cout << i << "->" << "-inf" << "\n";
			file << i << "->" << "-inf" << "\n";
		}
		else if (t_dist[i] == infinity) {
			std::cout << i << "->" << "inf" << "\n";
			file << i << "->" << "inf" << "\n";
		}
		else {
			std::cout << i << "->" << t_dist[i] << "\n";
			file << i << "->" << t_dist[i] << "\n";
		}
	}
	std::cout << std::endl;
	file.close();
}


//bellmanFord for adjencyList
double bellmanFord(std::shared_ptr<ListGraph> t_graph, int t_startNode, bool t_printSolution) {

	auto t_start = std::chrono::high_resolution_clock::now(); //start clock

	int* storeDistance = new int[t_graph->getV()];

	for (int iCell = 0; iCell < t_graph->getV(); ++iCell) {

		storeDistance[iCell] = infinity; //infinity
	}

	storeDistance[t_startNode] = 0;

	for (int i = 1; i < t_graph->getV(); ++i) {
		std::cout << i << "\n";
		for (int j = 0; j < t_graph->getV(); ++j) {

			std::shared_ptr<const Node> tmp = t_graph->getHeadOfGuard(j);

			while (tmp) {

				int u = j;
				int v = tmp->getNumber();
				int weight = tmp->getWeight();

				if (storeDistance[u] + weight < storeDistance[v]) {
					storeDistance[v] = storeDistance[u] + weight;
				}
				tmp = tmp->pm_next;

			}

		}
	}

	for (int i = 1; i < t_graph->getV(); ++i) {
		for (int j = 0; j < t_graph->getV(); ++j) {

			std::shared_ptr<const Node> tmp = t_graph->getHeadOfGuard(j);
			if (!tmp && storeDistance[j] > infinity/2) storeDistance[j] = infinity;

			while (tmp) {

				int u = j;
				int v = tmp->getNumber();
				int weight = tmp->getWeight();

				if (storeDistance[u] + weight < storeDistance[v]) {

					if (storeDistance[u] > infinity/2) {

						storeDistance[u] = infinity;
					}
					else {

						storeDistance[v] = neg_infinity; //negative inifinity
					}
				}
				else if (storeDistance[u] > infinity/2) {

					storeDistance[u] = infinity;
				}
				tmp = tmp->pm_next;
			}
		}
	}
	auto t_end = std::chrono::high_resolution_clock::now(); //stop clock

	if (t_printSolution) finalSolution(std::move(storeDistance), t_graph->getV(), t_startNode);
	delete[] storeDistance;
	return std::chrono::duration<double, std::milli>(t_end - t_start).count(); //return the time difference
}


//bellmanFord for adjencyMatrix
double bellmanFord(std::shared_ptr<MatrixGraph> t_graph, int t_startNode, bool t_printSolution) {

	auto t_start = std::chrono::high_resolution_clock::now(); //start clock

	int* storeDistance = new int[t_graph->getV()];

	for (int iCell = 0; iCell < t_graph->getV(); ++iCell) {

		storeDistance[iCell] = infinity; //infinity
	}

	storeDistance[t_startNode] = 0;

	for (int i = 1; i < t_graph->getV(); ++i) {
		for (int j = 0; j < t_graph->getV(); ++j) {
			for (int w = 0; w < t_graph->getV(); ++w) {
				if (t_graph->getWeight(j, w) != 0) {

					int u = j;
					int v = w;
					int weight = t_graph->getWeight(j, w);

					if (storeDistance[u] + weight < storeDistance[v]) {
						storeDistance[v] = storeDistance[u] + weight;
					}
				}
			}
		}
	}

	for (int i = 1; i < t_graph->getV(); ++i) {
		for (int j = 0; j < t_graph->getV(); ++j) {
			for (int w = 0; w < t_graph->getV(); ++w) {

				int foo_weight = t_graph->getWeight(j, w);

				if (foo_weight == 0 && storeDistance[j] > infinity/2) storeDistance[j] = infinity;

				if (foo_weight != 0) {

					int u = j;
					int v = w;
					int weight = foo_weight;

					if (storeDistance[u] + weight < storeDistance[v]) {

						if (storeDistance[u] > infinity/2) {

							storeDistance[u] = infinity;
						}
						else {

							storeDistance[v] = neg_infinity; //negative inifinity
						}
					}
					else if (storeDistance[u] > infinity/2) {

						storeDistance[u] = infinity;
					}
				}
			}
		}
	}
	auto t_end = std::chrono::high_resolution_clock::now(); //stop clock

	if (t_printSolution) finalSolution(std::move(storeDistance), t_graph->getV(), t_startNode);
	delete[] storeDistance;
	return std::chrono::duration<double, std::milli>(t_end - t_start).count(); //return the time difference
}
