#include <iostream>
#include <fstream>
#include "../lib/graph.cpp"

using namespace std;

const short MAX_COFLUENCY = 1;
const short MAX_ELEMENTALS = 3;

struct magician {
	string name = "Jhon";
	int ilegalSpells = 0;
};

// START OF DEBUG FUNCTIONS

void printGraphs(graph list[], int size) {
	for (int i = 0; i < size; i++) {
		list[i].print();
	}
}


void printSuspectThings(magician suspects[], int size) {
	printf("\n");
	for (int i = 0; i < size; i++) {
		cout << "Name: " << suspects[i].name << '\n';
		cout << "Ilegal spells: " << suspects[i].ilegalSpells << '\n';
		printf("\n");
	}
}

// END OF DEBUG FUNCTIONS

// Function to open a file and check if it opened correctly
ifstream checkFile(string fileName) {
	ifstream file(fileName);

	if (!file) {
		cerr << "\e[0;31mError: The file could not be opened\e[0m" << '\n';
		exit(1);
	}

	return file;
}

// Function to get the quantity of spells from the file
int getSpellsQuantity(ifstream &file) {
	string spellsLine = "";
	// It has to be the first function to be called to get the quantity correctly
	getline(file, spellsLine);
	int spellsQuantity = stoi(spellsLine);

	if (spellsQuantity < 1) {
		cerr << "\e[0;31mError: The number of spells is out of range\e[0m" << '\n';
		exit(1);
	}
	return spellsQuantity;
}

// Recursive function to get the list of suspects from the file (Yeah, I'm kind of a genius)
void getSuspectsList(ifstream &file, magician* &suspects, int &suspectsQuantity, int index = 0) {
    if (file.eof()) {
		suspectsQuantity--;
        suspects = new magician[suspectsQuantity];
        return;
    }

    string suspectLine;
    getline(file, suspectLine);
    suspectsQuantity++;

    getSuspectsList(file, suspects, suspectsQuantity, index + 1);

	if (index < suspectsQuantity) {
		suspects[index].name = suspectLine;
	}
}

// Function to read the spell list from the file
void readSpellList(ifstream &file, graph spells[], int spellsQuantity) {
	string wizardName, vertexTypes;
	int vertexQuantity, edgesQuantity, from, to, weight;
	char vertex;

    for (int i = 0; i < spellsQuantity; ++i) {
        getline(file, wizardName);
		spells[i].suspectName = wizardName;

        file >> vertexQuantity;
		file.ignore(); // Shitty C++ getline bug [ actually it's not a bug, it's a feature :) ]
		getline(file, vertexTypes);

        for (int j = 0; j < vertexQuantity; ++j) {
			vertex = (vertexTypes[j] != ' ')? vertexTypes[j]: '0'; // I asume that the vertex type is '0' if it's not specified
            spells[i].addVertex(j + 1, vertex);
        }

        file >> edgesQuantity;
		file.ignore();

        for (int j = 0; j < edgesQuantity; ++j) {
            file >> from >> to >> weight;
			file.ignore();
            spells[i].addEdge(from, to, weight);
        }
    }
}

// To know if the spells are valid (no shit sherlock)
void checkSpell(graph spells[], int spellIterator, int &cofluencyCounter, int &elementalsCounter, bool &isEnergeticRune) {
	vertex* actualRune = spells[spellIterator].vertices;

	for (int runeIterator = 0; runeIterator < spells[spellIterator].vertexQuantity; runeIterator++) {

		switch (actualRune->type) {
			case 'A': cofluencyCounter++; break;
			case 'I': elementalsCounter++; break;
			case 'Q': elementalsCounter++; break;
			case 'T': elementalsCounter++; break;
			case 'V': elementalsCounter++; break;
			case 'L': elementalsCounter++; break;
			case 'O': elementalsCounter++; break;
			case 'F': break;
			case 'B': break;
			case 'D': break;
			default: cout << "\n\e[0;31mSomething really bad happened, check your elementals runes...\e[0m\n"; break;
		}

		if (actualRune->next != nullptr) actualRune = actualRune->next;
	}

	string adjacentTypes = spells[spellIterator].getAdjacentTypes(actualRune);
	for (int adjacentIterator = 0; adjacentIterator < adjacentTypes.length(); adjacentIterator++) {
		if (adjacentTypes[spellIterator] != 'B') isEnergeticRune = false;
	}
}

// To know if the magicians are ilegal
void getIlegalMagicians(graph spells[], magician suspects[], int spellsQuantity, int suspectsQuantity) {

	for (int spellIterator = 0; spellIterator < spellsQuantity; spellIterator++) {
		int cofluencyCounter = 0, elementalsCounter = 0;
		string magicianName = spells[spellIterator].suspectName;
		bool isEnergeticRune = true;

		checkSpell(spells, spellIterator, cofluencyCounter, elementalsCounter, isEnergeticRune);

		for (int suspectIterator = 0; suspectIterator < suspectsQuantity; suspectIterator++) {
			if (suspects[suspectIterator].name != magicianName) continue;

			if (cofluencyCounter > MAX_COFLUENCY) {
				suspects[suspectIterator].ilegalSpells++;
				printf("\n\e[0;31mERROR: [%s] Cofluency counter exceeded the limit\e[0m\n", magicianName.c_str());
			}

			if (elementalsCounter > MAX_ELEMENTALS) {
				suspects[suspectIterator].ilegalSpells++;
				printf("\n\e[0;31mERROR: [%s] Elemental runes counter exceeded the limit\e[0m\n", magicianName.c_str());
			}

			if (!isEnergeticRune) {
				suspects[suspectIterator].ilegalSpells++;
				printf("\n\e[0;31mERROR: [%s] Energetic runes counter exceeded the limit\e[0m\n", magicianName.c_str());
			}
		}
	}
}


int main(int argc, char *argv[]) {
	// Open spell and suspect files
	ifstream spellsFile = checkFile("../build/spellList.in");
	ifstream suspectsFile = checkFile("../build/underInvestigation.in");

	// Get the list of suspects
	magician* suspects = nullptr;
	int suspectsQuantity = 0;
	getSuspectsList(suspectsFile, suspects, suspectsQuantity);

	// Get the list of spells
    int spellsQuantity = getSpellsQuantity(spellsFile);
    graph spells[spellsQuantity];
    readSpellList(spellsFile, spells, spellsQuantity);

	// Check if the spells are valid duh
	getIlegalMagicians(spells, suspects, spellsQuantity, suspectsQuantity);

	// Just debug stuff
	//printGraphs(spells, spellsQuantity);
	printSuspectThings(suspects, suspectsQuantity);

    spellsFile.close();
    suspectsFile.close();

	return 0;
}
