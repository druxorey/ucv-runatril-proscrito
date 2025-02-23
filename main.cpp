#include <iostream>
#include <fstream>

using namespace std;

ifstream checkFile(string fileName) {
	ifstream file(fileName);

	if (!file) {
		cerr << "\e[0;31mError: The file could not be opened\e[0m" << '\n';
		exit(1);
	}

	return file;
}


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


int main(int argc, char *argv[]) {
	ifstream spellFile = checkFile("spellList.in");
	ifstream suspectFile = checkFile("underInvestigation.in");

    int spellsQuantity = getSpellsQuantity(spellFile);


    spellFile.close();
    suspectFile.close();

	return 0;
}
