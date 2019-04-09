
struct node {

	int number, weight;
	node *next;
	node(int number, int weight, node *next) :number(number), weight(weight), next(next){}
};

class graph {

	node **head;
	int V, E, density;

	graph(int V, int E, int density);
};

graph::graph(int V, int E, int density) {

	head = new node*[V]();
	this->E = density * V * (V - 1) / 2;
	this->V = V;

	for (int i = 0; i < V; i++)
		head[i] = nullptr;
}


int main() {
	
}