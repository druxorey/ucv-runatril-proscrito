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


int main(int argc, char *argv[]) {
	ifstream spellFile = checkFile("spellList.in");
	ifstream suspectFile = checkFile("underInvestigation.in");


    spellFile.close();
    suspectFile.close();

	return 0;
}
