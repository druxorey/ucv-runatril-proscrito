#include <iostream>

using namespace std;

class edge {
    public:
        int weight;
		int to;  // Name of the destination vertex
        edge* next;  // Puntero a la siguiente arista en la lista de adyacencia

		// Constructor to initialize an edge with weight and type
        edge(int w, int t) : weight(w), to(t), next(nullptr) {}
};

class vertex {
    public:
		int adjacentVertex = 0;
        int name;
        char type;
        edge* edgeList;
        vertex* next;  // Pointer to the next vertex in the vertex list

        // Constructor to initialize a vertex with a name
        vertex(int n, char t) : name(n), type(t), edgeList(nullptr), next(nullptr) {}
};

class graph {
    public:
        vertex* vertices;  // Pointer to the first vertex in the vertex list
		string suspectName;
		int vertexQuantity = 0;

        // Constructor to initialize an empty graph
        graph() : vertices(nullptr) {}

        void addVertex(const int& name, const char& type);
        void addEdge(const int& from, const int& to, int weight);
		string getAdjacentTypes(vertex* v);
        void print();

    private:
        vertex* findVertex(const int& name);
};

// Method to find a vertex by name
vertex* graph::findVertex(const int& name) {
    vertex* v = vertices;  // Start from the first vertex
    while (v) {
        if (v->name == name) return v;  // Return the vertex if found
        v = v->next;  // Move to the next vertex in the list
    }
    return nullptr;  // Return nullptr if the vertex is not found
}

// Method to add a vertex to the graph
void graph::addVertex(const int &name, const char &type) {
    vertex* newVertex = new vertex(name, type);  // Create a new vertex
    newVertex->next = vertices;  // Point the new vertex to the current first vertex
    vertices = newVertex;  // Update the vertices pointer to the new vertex
	vertexQuantity++;
}

// Method to add an edge between two vertices
void graph::addEdge(const int& from, const int& to, int weight) {
    vertex* fromVertex = findVertex(from);  // Find the source vertex
    vertex* toVertex = findVertex(to);  // Find the destination vertex

    if (fromVertex && toVertex) {  // If both vertices exist
        edge* newEdge = new edge(weight, to);  // Create a new edge
        newEdge->next = fromVertex->edgeList;  // Point the new edge to the current edge list of the source vertex
        fromVertex->edgeList = newEdge;  // Update the edge list of the source vertex
		fromVertex->adjacentVertex++;

        // Since the graph is undirected, add the edge in the opposite direction as well
        newEdge = new edge(weight, from);  // Create another edge for the opposite direction
        newEdge->next = toVertex->edgeList;  // Point the new edge to the current edge list of the destination vertex
        toVertex->edgeList = newEdge;  // Update the edge list of the destination vertex
		toVertex->adjacentVertex++;
    }
}

// Method to print the graph
void graph::print() {
    vertex* v = vertices;  // Start from the first vertex
	printf("\nSuspect \e[0;34m%s\e[0m has the following connections:\n\n", suspectName.c_str());
    while (v) {
		printf("\e[0;32mVertex %d\e[0m with type \e[0;32m%c\e[0m connects to:\n", v->name, v->type);
        edge* e = v->edgeList;  // Get the edge list of the current vertex
        while (e) {
			vertex* connectedVertex = findVertex(e->to);  // Find the connected vertex by edge weight
			if (connectedVertex) {
				printf("    \e[0;33mVertex %d\e[0m with type \e[0;33m%c\e[0m through edge with weight \e[0;33m%d\e[0m\n", connectedVertex->name, connectedVertex->type, e->weight);
			}
			e = e->next;  // Move to the next edge in the adjacency list
        }
        v = v->next;  // Move to the next vertex in the vertex list
    }
}

// Get the types of the adjacent vertices
string graph::getAdjacentTypes(vertex* v) {
	string adjacentTypes = "";
	edge* e = v->edgeList;  // Get the edge list of the current vertex
	for (int i = 0; i < v->adjacentVertex; i++) {
		vertex* connectedVertex = findVertex(e->to);  // Find the connected vertex by edge weight
		if (connectedVertex) adjacentTypes += connectedVertex->type;
		e = e->next;  // Move to the next edge in the adjacency list
	}

	return adjacentTypes;
}
