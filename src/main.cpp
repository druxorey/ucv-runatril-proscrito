#include <iostream>
#include <fstream>
#include "../lib/graph.cpp"

using namespace std;

struct magician {
	string name = "Jhon";
	bool isIlegal = false;
};

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
void isSpellsValid(graph spells[], int spellsQuantity) {
	int cofluencyCounter = 0, elementalRunes = 0;

	for (int i = 0; i < spellsQuantity; i++) {
		vertex* v = spells->vertices;

		for (int j = 0; j < spells[i].vertexQuantity; j++) {

			switch (v->type) {
				case 'A': cofluencyCounter++; break;
				case 'I': elementalRunes++; break;
				case 'Q': elementalRunes++; break;
				case 'T': elementalRunes++; break;
				case 'V': elementalRunes++; break;
				case 'L': elementalRunes++; break;
				case 'O': elementalRunes++; break;
				case 'F': break;
				case 'B': break;
				case 'D': break;
				default: cout << "\n\e[0;31mSomething really bad happened, check your elementals runes...\e[0m\n"; break;
			}

			v = v->next;
		}
		if (cofluencyCounter > 1) cout << "\n\e[0;31mERROR: Cofluency counter exceeded the limit\e[0m\n";
		if (elementalRunes > 3) cout << "\n\e[0;31mERROR: Elemental runes exceeded the limit\e[0m\n";
	}
}


int main(int argc, char *argv[]) {
	// Open spell and suspect files
	ifstream spellFile = checkFile("../build/spellList.in");
	ifstream suspectFile = checkFile("../build/underInvestigation.in");

	// Get the list of suspects
	magician* suspects = nullptr;
	int suspectsQuantity = 0;
	getSuspectsList(suspectFile, suspects, suspectsQuantity);

	// Get the list of spells
    int spellsQuantity = getSpellsQuantity(spellFile);

    graph spells[spellsQuantity];
    readSpellList(spellFile, spells, spellsQuantity);

	// Check if the spells are valid duh
	isSpellsValid(spells, spellsQuantity);

	// Just for testing things
    spells[0].print();
	cout << suspects[0].isIlegal << endl;

    spellFile.close();
    suspectFile.close();

	return 0;
}
