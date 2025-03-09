#include <iostream>

#pragma once

template <typename datatype>
class node {
	public:
		datatype payload;
		node<datatype> *prev;
		node<datatype> *next;

		// Constructor to initialize the node with a payload and set prev and next to nullptr
		node(datatype payload) {
			this->payload = payload;
			next = nullptr;
			prev = nullptr;
		}
};

template <typename datatype>
class list {
	public:

		// Define an iterator class for the list
		class iterator {
		public:
			// Constructor to initialize the iterator with a node pointer
			iterator(node<datatype>* ptr = nullptr) : thisNode(ptr) {}

			// Dereference operator to access the payload of the node
			datatype& operator*() const { return thisNode->payload; }
			// Arrow operator to access the payload of the node
			datatype* operator->() const { return &(thisNode->payload); }

			// Pre-increment operator to move the iterator to the next node
			iterator& operator++() {
				thisNode = thisNode->next;
				return *this;
			}

			// Post-increment operator to move the iterator to the next node
			iterator operator++(int) {
				iterator temporal = *this;
				thisNode = thisNode->next;
				return temporal;
			}

			// Pre-decrement operator to move the iterator to the previous node
			iterator& operator--() {
				thisNode = thisNode->prev;
				return *this;
			}

			// Post-decrement operator to move the iterator to the previous node
			iterator operator--(int) {
				iterator temporal = *this;
				thisNode = thisNode->prev;
				return temporal;
			}

			// Equality operator to compare two iterators
			bool operator==(const iterator& other) const { return thisNode == other.thisNode; }
			// Inequality operator to compare two iterators
			bool operator!=(const iterator& other) const { return thisNode != other.thisNode; }

		private:
			node<datatype>* thisNode;
			friend class list<datatype>; // Allow the list class to access private members
		};

		list();
		~list();

		// Return an iterator to the first node
		iterator begin() const { return iterator(_first); }
		// Return an iterator to the end (nullptr)
		iterator end() const { return iterator(nullptr); }

		bool empty() const; // Check if the list is empty
		int size() const; // Return the size of the list
		void push_back(const datatype& payload); // Add a node with the given payload to the end of the list
		void push_front(const datatype& payload); // Add a node with the given payload to the front of the list
		void erase(iterator position); // Remove the node at the given iterator position
		void print() const; // Print the contents of the list
		iterator insert(iterator position, const datatype& payload); // Insert a node with the given payload at the given iterator position

	private:
		node<datatype>* _first; // Pointer to the first node
		node<datatype>* _last; // Pointer to the last node
		int _size; // Number of nodes in the list
};

// Constructor implementation
template <typename datatype>
list<datatype>::list() : _first(nullptr), _last(nullptr), _size(0) {}

// Destructor implementation
template <typename datatype>
list<datatype>::~list() {
    while (!empty()) {
        erase(begin());
    }
}

// Check if the list is empty
template <typename datatype>
bool list<datatype>::empty() const {
    return _first == nullptr;
}

// Return the size of the list
template <typename datatype>
int list<datatype>::size() const {
    return _size;
}

// Add a node with the given payload to the end of the list
template <typename datatype>
void list<datatype>::push_back(const datatype& payload) {
    insert(end(), payload);
}

// Add a node with the given payload to the front of the list
template <typename datatype>
void list<datatype>::push_front(const datatype& payload) {
    insert(begin(), payload);
}

// Insert a node with the given payload at the given iterator position
template <typename datatype>
typename list<datatype>::iterator list<datatype>::insert(iterator pos, const datatype& payload) {
    node<datatype>* newNode = new node<datatype>(payload);
    _size++;

    if (empty()) {
        _first = newNode;
        _last = newNode;
        return iterator(newNode); 
	}

    node<datatype>* current = pos.thisNode; // Get the current node

    if (current == nullptr) { // If the position is at the end
        newNode->prev = _last;
        _last->next = newNode; // Set the next node of the last node
        _last = newNode; // Update the last node

    } else { // If the position is not at the end
        newNode->next = current; // Set the next node of the new node
        newNode->prev = current->prev; // Set the previous node of the new node

        if (current->prev != nullptr) { // If the current node is not the first node
            current->prev->next = newNode; // Update the next node of the previous node
        }
        current->prev = newNode; // Update the previous node of the current node
        if (_first == current) { // If the current node is the first node
            _first = newNode; // Update the first node
        }
    }

    return iterator(newNode); // Return an iterator to the new node
}

// Remove the node at the given iterator position
template <typename datatype>
void list<datatype>::erase(iterator pos) {
    node<datatype>* current = pos.thisNode; // Get the current node
    if (current == nullptr) return; // If the current node is null, return

    _size--; // Decrement the size of the list

    if (current == _first) _first = current->next; // If the current node is the first node, update the first node
    if (current == _last) _last = current->prev; // If the current node is the last node, update the last node
    if (current->prev != nullptr) current->prev->next = current->next; // Update the next node of the previous node
    if (current->next != nullptr) current->next->prev = current->prev; // Update the previous node of the next node

    delete current; // Delete the current node
}

// Print the contents of the list
template <typename datatype>
void list<datatype>::print() const {
    int nodeIndex = 1; // Initialize the node index
    int maxNumber = std::to_string(this->size()).length(); // Get the length of the size of the list
    for (auto iterator = this->begin(); iterator != this->end(); ++iterator) { // Iterate through the list
        printf("Node %0*d: [\e[0;33m%d\e[0m]\n", maxNumber, nodeIndex, *iterator); // Print the node index and payload
        nodeIndex++; // Increment the node index
    }
}
