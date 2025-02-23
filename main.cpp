#include <iostream>
#include <fstream>

using namespace std;

struct coordinates {
	int x;
	int y;
	int z;
};

struct spell {
	string name;
	int graphVertex;
	string typeVertex;
	int edges;
	coordinates edgeCoordinates;
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


int main(int argc, char *argv[]) {
	// Open spell and suspect files
	ifstream spellFile = checkFile("spellList.in");
	ifstream suspectFile = checkFile("underInvestigation.in");

    // Get the quantity of spells
    int spellsQuantity = getSpellsQuantity(spellFile);
	spell spells[spellsQuantity];

	// Get the list of suspects
	string* suspects = nullptr;
	int suspectsQuantity = 0;
	getSuspectsList(suspectFile, suspects, suspectsQuantity);

    spellFile.close();
    suspectFile.close();

	return 0;
}
