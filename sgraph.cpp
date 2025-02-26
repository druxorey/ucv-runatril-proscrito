#include <iostream>

using namespace std;

class edge {
    public:
        int weight;
        char type;
        edge* next;  // Pointer to the next edge in the adjacency list

        // Constructor to initialize an edge with weight and type
        edge(int w, char t) : weight(w), type(t), next(nullptr) {}
};

class vertex {
    public:
        int name;
        edge* edgeList;
        vertex* next;  // Pointer to the next vertex in the vertex list

        // Constructor to initialize a vertex with a name
        vertex(int n) : name(n), edgeList(nullptr), next(nullptr) {}
};

class graph {
    public:
        vertex* vertices;  // Pointer to the first vertex in the vertex list
		string suspectName;

        // Constructor to initialize an empty graph
        graph() : vertices(nullptr) {}

        void addVertex(const int& name);
        void addEdge(const int& from, const int& to, int weight, char type);
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
void graph::addVertex(const int& name) {
    vertex* newVertex = new vertex(name);  // Create a new vertex
    newVertex->next = vertices;  // Point the new vertex to the current first vertex
    vertices = newVertex;  // Update the vertices pointer to the new vertex
}

// Method to add an edge between two vertices
void graph::addEdge(const int& from, const int& to, int weight, char type) {
    vertex* fromVertex = findVertex(from);  // Find the from vertex
    vertex* toVertex = findVertex(to);  // Find the to vertex

    if (fromVertex && toVertex) {  // If both vertices exist
        edge* newEdge = new edge(weight, type);  // Create a new edge
        newEdge->next = fromVertex->edgeList;  // Point the new edge to the current edge list of from vertex
        fromVertex->edgeList = newEdge;  // Update the edge list of from vertex

        // Since the graph is undirected, add the edge in the opposite direction as well
        newEdge = new edge(weight, type);  // Create another edge for the opposite direction
        newEdge->next = toVertex->edgeList;  // Point the new edge to the current edge list of to vertex
        toVertex->edgeList = newEdge;  // Update the edge list of to vertex
    }
}

// Method to print the graph
void graph::print() {
    vertex* v = vertices;  // Start from the first vertex
	printf("\nSuspect \e[0;34m%s\e[0m has the following connections:\n\n", suspectName.c_str());
    while (v) {
		printf("\e[0;32mVertex %d\e[0m connects to:\n", v->name);
        edge* e = v->edgeList;  // Get the edge list of the current vertex
        while (e) {
			printf(" - Vertex with weight \e[0;33m%d\e[0m and type \e[0;33m%c\e[0m\n", e->weight, e->type);
            e = e->next;  // Move to the next edge in the adjacency list
        }
        v = v->next;  // Move to the next vertex in the vertex list
    }
}
