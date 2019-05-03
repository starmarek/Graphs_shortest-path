#include "bellman_algorithms.hpp"

static int  infinity = 1000000; //because of the small interval in which weight of the edge can fit
static int  neg_infinity = -1000000;	//it is ok to set inf to the one million. If you want to make this
										//interval bigger, you should also change this static variable to a bigger one


//prints to console and to just created output file the result of bellmanFord algorithm
void finalSolution(std::string dupa[], int t_dist[], int t_n, int t_startNode) {

	std::ofstream file("Output.txt");
	
	std::cout << std::string(20, '-') << "\n   FinalSolution\n\n";
	std::cout << "The starting node was -> " << t_startNode << "\n\n";

	file << std::string(20, '-') << "\n   FinalSolution\n\n";
	file << "The starting node was -> " << t_startNode << "\n\n";

	for (int i = 0; i < t_n; ++i) {
		 
		if (t_dist[i] == neg_infinity) {
			std::cout << i << "->" << "-inf\n";
			file << i << "->" << "-inf\n";
			continue; //continue so program wont print path to this node (look at the end of the lop)
		}
		else if (t_dist[i] == infinity) {
			std::cout << i << "->" << "inf\n";
			file << i << "->" << "inf\n";
			continue; //the same story right there
		}
		//if there is no inf or neg_inf print the shortest distance
		else {
			std::cout << i << "->" << t_dist[i];
			file << i << "->" << t_dist[i];
		}
		//printing shortest path stored in std::string object
		std::cout << "   The shortest path: " << dupa[i] << i;
		file << "   The shortest path: " << dupa[i] << i;
		
		std::cout << std::endl;
		file << std::endl;
	}
	std::cout << std::endl;
	file.close();
}


//bellmanFord for adjencyList
double bellmanFord(std::shared_ptr<ListGraph> t_graph, int t_startNode, bool t_printSolution) {

	std::string* storePath = new std::string[t_graph->getV()]; //storage for path strings 

	auto t_start = std::chrono::high_resolution_clock::now(); //start clock

	int* storeDistance = new int[t_graph->getV()]; //storage for shotrtest distances
	
	for (int iCell = 0; iCell < t_graph->getV(); ++iCell) {

		storeDistance[iCell] = infinity; //init all distances with inf
	}

	storeDistance[t_startNode] = 0; //and source one with 0

	for (int i = 1; i < t_graph->getV(); ++i) { //relax edges v-1 times
		for (int j = 0; j < t_graph->getV(); ++j) { //for all guards

			std::shared_ptr<const Node> tmp = t_graph->getHeadOfGuard(j);

			while (tmp) { //as long as current node exists

				int u = j;
				int v = tmp->getNumber();
				int weight = tmp->getWeight();

				if (storeDistance[u] + weight < storeDistance[v]) { //checing if shorter path is found
					storeDistance[v] = storeDistance[u] + weight;
					
					if (t_printSolution) { //if user want a solution to be printed than store path 
						
						storePath[v].clear();
						storePath[v].append(storePath[u] + std::to_string(u) + "->");
					}
				}
				tmp = tmp->pm_next; //going to the next node in the list
			}

		}
	}

	//checking for negative cycles right now
	for (int i = 1; i < t_graph->getV(); ++i) {
		for (int j = 0; j < t_graph->getV(); ++j) {

			std::shared_ptr<const Node> tmp = t_graph->getHeadOfGuard(j);

			if (!tmp && storeDistance[j] > infinity/2) storeDistance[j] = infinity; //after first part of the algorithm
						//if there was a negative cycle, and this (j) node is disconnected from the source then distances
						//are going to be corrupted and instead of infinity (1000000) there will be something like 9999992. 
						//thats why we need to manually set it to infinity. This is done by detecting > inf/2 so the the corrupted
						//distance sholud be interpreted correctly and not confused with huge (correct) distance. Its also in 
						//the middle so huge corruption won't mess it up either. Thats why its best to distinguish by 
						//the middle event.
			while (tmp) {

				int u = j;
				int v = tmp->getNumber();
				int weight = tmp->getWeight();
				if (storeDistance[u] + weight < storeDistance[v]) {

					if (storeDistance[u] > infinity/2) storeDistance[u] = infinity;  //this time, it is just a protection 
											//before adding negative inf when the node is disconnected from the source anyway
					else storeDistance[v] = neg_infinity;

				}
				else if (storeDistance[u] > infinity/2) storeDistance[u] = infinity;  //the same as the (if) before (while loop)
										//but this time in case of existing nodes in this guard. The other (if) was intentionally
										//before (while), because it would have never be executed and 
										//would have missed one case when it was needed to change to infinity
				tmp = tmp->pm_next;
			}
		}
	}
	auto t_end = std::chrono::high_resolution_clock::now(); //stop clock

	//prinitng solution if user wanted to
	if (t_printSolution) finalSolution(std::move(storePath), std::move(storeDistance), t_graph->getV(), t_startNode);
	delete[] storeDistance;
	return std::chrono::duration<double, std::milli>(t_end - t_start).count(); //return the time difference
}


//bellmanFord for adjencyMatrix (the same algorithm as for adjencyList)
double bellmanFord(std::shared_ptr<MatrixGraph> t_graph, int t_startNode, bool t_printSolution) {
	
	std::string* storePath = new std::string[t_graph->getV()];

	auto t_start = std::chrono::high_resolution_clock::now(); //start clock

	int* storeDistance = new int[t_graph->getV()];

	for (int iCell = 0; iCell < t_graph->getV(); ++iCell) {

		storeDistance[iCell] = infinity; //infinity
	}

	storeDistance[t_startNode] = 0;

	for (int i = 1; i < t_graph->getV(); ++i) {
		for (int j = 0; j < t_graph->getV(); ++j) {
			for (int w = 0; w < t_graph->getV(); ++w) {

				int u = j;
				int v = w;
				int weight = t_graph->getWeight(j, w);
				if (storeDistance[u] + weight < storeDistance[v]) {
					storeDistance[v] = storeDistance[u] + weight;

					if (t_printSolution) {

						storePath[v].clear();
						storePath[v].append(storePath[u] + std::to_string(u) + "->");
					}
				}
			}
		}
	}
	for (int i = 1; i < t_graph->getV(); ++i) {
		for (int j = 0; j < t_graph->getV(); ++j) {
			for (int w = 0; w < t_graph->getV(); ++w) {

				int foo_weight = t_graph->getWeight(j, w);
			
				if (storeDistance[j] > infinity / 2) storeDistance[j] = infinity;

				int u = j;
				int v = w;
				int weight = foo_weight;
				if (storeDistance[u] + weight < storeDistance[v]) {
				
					if (storeDistance[u] > infinity / 2) storeDistance[u] = infinity;
					else if (weight == infinity) continue; //if this node(j) have no connection with node(w)
					else storeDistance[v] = neg_infinity;
				}
				else if (storeDistance[u] > infinity/2) storeDistance[u] = infinity;	
			}
		}
	}
	auto t_end = std::chrono::high_resolution_clock::now(); //stop clock

	if (t_printSolution) finalSolution(std::move(storePath), std::move(storeDistance), t_graph->getV(), t_startNode);
	delete[] storeDistance;
	return std::chrono::duration<double, std::milli>(t_end - t_start).count(); //return the time difference
}
