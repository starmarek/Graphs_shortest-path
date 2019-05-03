#include "list_graph.hpp"


ListGraph::ListGraph(int t_V, double t_density)
	:Graph(t_V, static_cast<int>(t_density* t_V* (t_V - 1)), t_density), //the t_E (edges) is an official formula to be calculated
	prgm_guards(std::move(std::shared_ptr<ListGuard[]>(new ListGuard[t_V]))) {		//depending on density and vertices

	//fill guard array with null pointers
	for (int i = 0; i < t_V; ++i) {
		prgm_guards[i].pm_head = nullptr, prgm_guards[i].pm_tail = nullptr;
	}
}


std::shared_ptr<Node> ListGraph::getHeadOfGuard(int t_index) const {

	return prgm_guards[t_index].pm_head;
}

bool ListGraph::detectIfExist(int t_guardNumber, int t_nodeNumber) const {

	std::shared_ptr<Node> tmp = prgm_guards[t_guardNumber].pm_head; //getting head of given guard

	while (tmp) { //searching through nodes as long as given number is found
		if (tmp->getNumber() == t_nodeNumber) return true;
		tmp = tmp->pm_next; //go to next node
	}
	return false; //if not found, return false
}


void ListGraph::addNodeEnd(int t_number, int t_weight, int t_guardNumber) const {

	std::shared_ptr<Node> newNode = std::make_shared<Node>(t_number, t_weight, //node initialization
															prgm_guards[t_guardNumber].pm_tail, nullptr);
	//adding node back
	prgm_guards[t_guardNumber].pm_tail = newNode;

	//if previous node exists set its next pointer to newnode
	if (newNode->pm_previous.lock()) newNode->pm_previous.lock()->pm_next = std::move(newNode);
	else prgm_guards[t_guardNumber].pm_head = std::move(newNode);
	//else set guard head to newnode because newnode is a first element in the list
}


void ListGraph::fillGraph(bool allowLoops) const {

	if (m_density == 1) {  //full graph case
		for (int iGuard = 0; iGuard < m_V; ++iGuard) { //for each guard
			for (int jNode = 0; jNode < m_V; ++jNode) { //connect this guard with every other node
				if (iGuard != jNode) { //to avoid loops to the same node
					int foo_weight = rand() % 20 - 1;
					while (foo_weight == 0) {
						foo_weight = rand() % 20 - 1;
					}
					addNodeEnd(jNode, foo_weight, iGuard); 
				}
			}
		}
	}
	else { //other densities; the same algorithm as in the matrixgraph -> for hints look there

		int foo_edges = m_E;

		while (foo_edges) {

			int guardNode = rand() % m_V;
			int nodeNode = rand() % m_V;
			
			if (!detectIfExist(guardNode, nodeNode)) {

				int foo_weight = rand() % 20 - 1;
				while (foo_weight == 0) {
					foo_weight = rand() % 20 - 1;
				}
				
				if (guardNode != nodeNode) {

					addNodeEnd(nodeNode, foo_weight, guardNode);
					--foo_edges;
				}
				else if (allowLoops) {
					addNodeEnd(nodeNode, foo_weight, guardNode);
					--foo_edges;
				}
			}
		}
	}
}


void ListGraph::printGraph() const {

	std::cout << std::string(30, '-') << "\n  AdjencyList Representation\n\n";

	for (int iGuard = 0; iGuard < m_V; ++iGuard) {
		
		//number of guard
		std::cout << iGuard;
		std::shared_ptr<Node> tmp = prgm_guards[iGuard].pm_head;
		
		//as long as there is a connection, print it
		while (tmp != nullptr) {
			std::cout << "->" << "[" << tmp->getNumber() << "|" << tmp->getWeight() << "]";
			tmp = tmp->pm_next;
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}


int ListGraph::readFromFile() {

	int start, source, destination, weight;

	std::ifstream file("Input.txt");
	if (!file.is_open()) {
		throw "INPUT LIST NOT OPEN!";
		return 1;
	}

	//attributes and arrays initialization (apart of start -> starting node, which is not an attribute)
	file >> m_E >> m_V >> start;
	prgm_guards = std::shared_ptr<ListGuard[]>(new ListGuard[m_V]);
	for (int i = 0; i < m_V; ++i) {
		prgm_guards[i].pm_head = nullptr, prgm_guards[i].pm_tail = nullptr;
	}

	//adding all connections
	for (int iEdge = 0; iEdge < m_E; ++iEdge) {

		file >> source >> destination >> weight;
		addNodeEnd(destination, weight, source);
	}
	file.close();

	return start; //return starting node
}

//the same as martixGraph
void ListGraph::createInput(int t_startNode) const {

	std::ofstream file("CreatedInput.txt");
	if (!file.is_open()) {
		throw  "CREATED INPUT LIST NOT OPEN!";
		return;
	}

	file << m_E << " " << m_V << " " << t_startNode << "\n";
	for (int iGuard = 0; iGuard < m_V; ++iGuard) {

		std::shared_ptr<Node> tmp = prgm_guards[iGuard].pm_head;
		while(tmp) {
			file << iGuard << " ";
			file << tmp->getNumber() << " ";
			file << tmp->getWeight() << "\n";
			tmp = tmp->pm_next;
		}
	}
	file.close();
}
