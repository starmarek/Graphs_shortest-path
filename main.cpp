#include <time.h>
#include <stdlib.h>
#include <iostream>


//represents node of graph as an element of doubly linked list
struct	Node {

private:	
	static int s_numCreated; 
	static int s_numDestroyed;
							// number of node, and weight of edge that is required to get to this node (depending on
	int m_number, m_weight; // number of guard [each guard represents consecutive nodes of graph -> look adjencylist])


public:
	std::weak_ptr<Node>  pm_previous; //pointers to 
	std::shared_ptr<Node> pm_next; 	  //adjacent nodes

	const int& getNumber() const { return m_number; };
	const int& getWeight() const { return m_weight; };
	static void informAboutNodes();
	Node(int t_number, int t_weight, std::weak_ptr<Node> pt_previous, std::shared_ptr<Node> pt_next)
		:m_number(t_number), m_weight(t_weight), pm_previous(pt_previous), pm_next(pt_next) 
		{++s_numCreated;}
	~Node() {++s_numDestroyed; }
};

int Node::s_numCreated = 0;
int Node::s_numDestroyed = 0;


//represents element that always points to the end and beginning of doubly linked list
struct ListGuard {

private:
	static int s_numCreated;
	static int s_numDestroyed;

public:
	std::shared_ptr<Node> pm_head; //pointer to beginning 
	std::shared_ptr<Node> pm_tail; //pointer to end 

	static void informAboutGuards();
	ListGuard() { ++s_numCreated; }
	~ListGuard() { ++s_numDestroyed; }
};

int ListGuard::s_numCreated = 0;
int ListGuard::s_numDestroyed = 0;


//graph in an adjency list representation 
class ListGraph {

	std::shared_ptr<ListGuard[]> prgm_guards; //dynamically allocated array of guards
	int m_V, m_E, m_density;	 //V-number of vertices, E-number of edges									

public:
	void fillGraph() const; //fills graph with nodes
	void addNodeEnd(int t_number, int t_weight, int t_guardNumber) const; 
	const int findWeight(int t_nodeNumber, int t_guardNumber) const; //finds weight of desired edge  
	void printGraph() const; 
	ListGraph(int t_V, int t_density);
};


void Node::informAboutNodes() {

	std::cout << "Created Nodes: " << s_numCreated << std::endl;
	std::cout << "Destroyed Nodes: " << s_numDestroyed << std::endl;
}

void ListGuard::informAboutGuards() {

	std::cout << "Created Guards: " << s_numCreated << std::endl;
	std::cout << "Destroyed Guards: " << s_numDestroyed << std::endl;
}


//when edges are beeing repeated in adjency list setup we need to find its weight that already's been initialized
//example-> we are currently setting connection of node 7 to 0 
//we pass to the function reverse numbers ->guardnumber of 0 and node 7.
//thanks to that function searches for an existiing connection 0 to 7 since we fill graph in ascending order
const int ListGraph::findWeight(int t_nodeNumber, int t_guardNumber) const {

		//start from head of a given guard
		std::shared_ptr<Node> tmp = prgm_guards[t_guardNumber].pm_head;

		//scroll through nodes as long as u wont find matching node number
		while (tmp->getNumber() != t_nodeNumber) tmp = tmp->pm_next;

		return tmp->getWeight();	
}


void ListGraph::addNodeEnd(int t_number, int t_weight, int t_guardNumber) const {

	std::shared_ptr<Node> newNode = std::make_shared<Node>(t_number, t_weight, 
															prgm_guards[t_guardNumber].pm_tail, nullptr);
	//adding node back
	prgm_guards[t_guardNumber].pm_tail = newNode;

	//if previous node exists set its next pointer to newnode
	if (newNode->pm_previous.lock()) {
		newNode->pm_previous.lock()->pm_next = newNode;
	}
	else { //else set guard head to newnode because newnode is a first element in the list
		prgm_guards[t_guardNumber].pm_head = newNode;
	}
}


// first check the density of constructed graph and then performs node filling, depending on this density
// 1 case -> when the graph is full we need to connect every node to every node
// 2 case -> other densities have totaly random node connections, including loops from node to the same node
void ListGraph::fillGraph() const {

	if (m_density == 1) {  //full graph case
		for (int iGuard = 0; iGuard < m_V; ++iGuard) { //for each guard
			for (int jNode = 0; jNode < m_V; ++jNode) { //for each node in the list
				
				if (jNode == iGuard) { //skip the node which is corresponding to the actual guard number (we are setting
					continue; //now the connections of iGuard node to other nodes so it cant be repeated)
				}

				if (jNode < iGuard) { //if the edge was already set we need to find its existing weight

					addNodeEnd(jNode, findWeight(iGuard, jNode), iGuard); //add node with existing edge weight
					continue;
				}
				addNodeEnd(jNode, rand() % 500 + 1, iGuard); //add node with totally new edge
			}
		}
	}
	else { //other densities

	}
}


ListGraph::ListGraph(int t_V, int t_density) 
	:m_density(t_density), m_V(t_V), m_E(t_density* t_V* (t_V - 1) / 2),  
	 prgm_guards(std::shared_ptr<ListGuard[]>(new ListGuard[t_V])) {

	//fill guard array with null pointers
	for (int i = 0; i < t_V; ++i) {
		prgm_guards[i].pm_head = nullptr, prgm_guards[i].pm_tail = nullptr;
	}
}


void ListGraph::printGraph() const {
	
	for (int iGuard = 0; iGuard < m_V; ++iGuard) {
		std::cout << iGuard;
		std::shared_ptr<Node> tmp = prgm_guards[iGuard].pm_head;
		while (tmp != nullptr) {
			std::cout << "->" << "[" << tmp->getNumber() << "|" 
					  << tmp->getWeight() << "]";
			tmp = tmp->pm_next;
		}
		std::cout << std::endl;
	}
}


int main() {

	{
		srand((unsigned int)time(NULL));
		ListGraph gp(5, 1);
		gp.fillGraph();
		gp.printGraph();
	}
	Node::informAboutNodes();
	ListGuard::informAboutGuards();
}