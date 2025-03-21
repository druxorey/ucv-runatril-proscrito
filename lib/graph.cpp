#include <iostream>
#pragma once

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
		int value;
		char type;
		int adjacentVertex;
		edge* edgeList;
		vertex* next;  // Pointer to the next vertex in the vertex list

        // Default constructor
		vertex() : value(0), type(0), edgeList(nullptr), next(nullptr), adjacentVertex(0) {}
		// Constructor to initialize a vertex with a name
		vertex(int n, char t) : value(n), type(t), edgeList(nullptr), next(nullptr), adjacentVertex(0) {}
};

class graph {
	public:
		string name;  // Hes just Master Chief
        string spellName;
		string type;
		bool vality;
		int vertexQuantity;
		vertex* vertices;  // Pointer to the first vertex in the vertex list

		// Constructor to initialize an empty graph
		graph() : vertices(nullptr), name("Jhon"), spellName(""), type("Arcano"), vertexQuantity(0), vality(true) {}

		void addVertex(const int& name, const char& type);
		void addEdge(const int& from, const int& to, int weight);
		string getAdjacentTypes(vertex* actualVertex);
		void print();
		vertex* findVertex(const int& name);
};

// Method to find a vertex by name
vertex* graph::findVertex(const int& name) {
	vertex* actualVertex = vertices;  // Start from the first vertex
	while (actualVertex) {
		if (actualVertex->value == name) return actualVertex;  // Return the vertex if found
		actualVertex = actualVertex->next;  // Move to the next vertex in the list
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

// Get the types of the adjacent vertices
string graph::getAdjacentTypes(vertex* actualVertex) {
	string adjacentTypes = "";
	edge* actualEdge = actualVertex->edgeList;  // Get the edge list of the current vertex

	for (int i = 0; i < actualVertex->adjacentVertex; i++) {
		vertex* connectedVertex = findVertex(actualEdge->to);  // Find the connected vertex by edge weight
		if (connectedVertex) adjacentTypes += connectedVertex->type;

		actualEdge = actualEdge->next;  // Move to the next edge in the adjacency list
	}

	return adjacentTypes;
}

// Method to print the graph
void graph::print() {
	vertex* actualVertex = vertices;  // Start from the first vertex
	printf("\n\e[0;34mGraph of the suspect %s\e[0m\n", name.c_str());
	printf("\e[0;34mSpell type: %s\e[0m\n", type.c_str());

	while (actualVertex) {
		printf("\e[0;32mVertex %d\e[0m with type \e[0;32m%c\e[0m connects to:\n", actualVertex->value, actualVertex->type);
		edge* actualEdge = actualVertex->edgeList;  // Get the edge list of the current vertex

		while (actualEdge) {
			vertex* connectedVertex = findVertex(actualEdge->to);  // Find the connected vertex by edge weight
			if (connectedVertex) printf("	\e[0;33mVertex %d\e[0m with type \e[0;33m%c\e[0m through edge with weight \e[0;33m%d\e[0m\n", connectedVertex->value, connectedVertex->type, actualEdge->weight);
			actualEdge = actualEdge->next;  // Move to the next edge in the adjacency list
		}
		actualVertex = actualVertex->next;  // Move to the next vertex in the vertex list
	}
}
