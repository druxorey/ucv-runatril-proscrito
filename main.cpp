#include <iostream>
#include <fstream>

using namespace std;

struct coordinates {
	int x;
	int y;
	int z;
};

struct spell {
	string suspectName;
	int graphVertex;
	string typeVertex;
	int edgesQuantity;
	coordinates* edgeCoordinates;
};





// BEGIN OF DEBUG FUNCTIONS

void printSpellsList(spell spells[], int spellsQuantity) {
    for (int i = 0; i < spellsQuantity; i++) {
        cout << "Suspect Name: " << spells[i].suspectName << endl;
        cout << "Graph Vertex: " << spells[i].graphVertex << endl;
        cout << "Type Vertex: " << spells[i].typeVertex << endl;
        cout << "Number of Edges: " << spells[i].edgesQuantity << endl;
        cout << "Edge Coordinates:" << endl;
        for (int j = 0; j < spells[i].edgesQuantity; j++) {
            cout << "  (" << spells[i].edgeCoordinates[j].x << ", "
                 << spells[i].edgeCoordinates[j].y << ", "
                 << spells[i].edgeCoordinates[j].z << ")" << endl;
        }
        cout << "------------------------" << endl;
    }
}

//END OF DEBUG FUNCTIONS





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
void getSuspectsList(ifstream &file, string* &suspects, int &suspectsQuantity, int index = 0) {
    if (file.eof()) {
		suspectsQuantity--;
        suspects = new string[suspectsQuantity];
        return;
    }

    string suspectLine;
    getline(file, suspectLine);
    suspectsQuantity++;

    getSuspectsList(file, suspects, suspectsQuantity, index + 1);

	if (index < suspectsQuantity) {
		suspects[index] = suspectLine;
	}
}


// Function to get the list of spells from the file
void getSpellsList(ifstream &file, spell spells[], int spellsQuantity) {
	string spellLine = "";

	for(int i = 0; i < spellsQuantity; i++) {
		getline(file, spellLine);
		spells[i].suspectName = spellLine;
		file >> spells[i].graphVertex;
		file >> spells[i].typeVertex;
		file >> spells[i].edgesQuantity;
		spells[i].edgeCoordinates = new coordinates[spells[i].edgesQuantity];
		for(int j = 0; j < spells[i].edgesQuantity; j++) {
			file >> spells[i].edgeCoordinates[j].x;
			file >> spells[i].edgeCoordinates[j].y;
			file >> spells[i].edgeCoordinates[j].z;
		}
	}
}


int main(int argc, char *argv[]) {
	// Open spell and suspect files
	ifstream spellFile = checkFile("spellList.in");
	ifstream suspectFile = checkFile("underInvestigation.in");

    // Get the quantity of spells
    int spellsQuantity = getSpellsQuantity(spellFile);
	spell spells[spellsQuantity];

	// Get the list of spells
	getSpellsList(spellFile, spells, spellsQuantity);
	printSpellsList(spells, spellsQuantity);

	// Get the list of suspects
	string* suspects = nullptr;
	int suspectsQuantity = 0;
	getSuspectsList(suspectFile, suspects, suspectsQuantity);

    spellFile.close();
    suspectFile.close();

	return 0;
}
