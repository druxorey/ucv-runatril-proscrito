#include <iostream>
#include <fstream>
#include "../lib/graph.cpp"
#include "../lib/list.cpp"
#include "../lib/processed_list.cpp"

using namespace std;

const short MAX_COFLUENCY = 1;
const short MAX_ELEMENTALS = 3;

struct magician {
	string name = "Jhon"; // It's Master Chief's again???
	int ilegalSpells = 0;
};


// START OF DEBUG FUNCTIONS

void printGraphs(graph list[], int size) {
	for (int i = 0; i < size; i++) {
		list[i].print();
		printf("\n --- This spell is %s", (list[i].vality)? "\e[0;32mvalid\e[0m\n": "\e[0;31minvalid\e[0m\n");
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
		suspects[index].ilegalSpells = 3;
	}
}

// Just because I'm a genius (I shouldn't have done it this way)
void addSuspect(magician* &suspects, int &suspectsQuantity, const string &name) {
	printf("\n\e[0;33mWARNING: [%s] is not in the list of suspects\e[0m\n", name.c_str());

    // Create a new array with increased size
    magician* newSuspects = new magician[suspectsQuantity + 1];

    // Copy existing suspects to the new array
    for (int i = 0; i < suspectsQuantity; ++i) {
        newSuspects[i] = suspects[i];
    }

    // Add the new suspect
    newSuspects[suspectsQuantity].name = name;
    newSuspects[suspectsQuantity].ilegalSpells = 0;

    suspectsQuantity++;

    // Delete the old array and update the pointer (Do not check O complexity)
    delete[] suspects;
    suspects = newSuspects;
}


void readSpellList(ifstream &file, graph spells[], int spellsQuantity) {
	string wizardName, vertexTypes;
	int vertexQuantity, edgesQuantity, from, to, weight;
	char vertex;

	for (int i = 0; i < spellsQuantity; ++i) {
		getline(file, wizardName);
		spells[i].name = wizardName;

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


bool alreadyVisited(vertex* visited[], int n, vertex* actual) {
	for (int i = 0; i < n; i++) {
		if (visited[i] == actual) return true;
	}
	return false;
}


int checkBiggestCircle(vertex* visited[], graph spell, vertex* initial, vertex* actual, int count, int max, int vertexVisited=0) {
	if (actual == initial) {
		if (count!=0) return count; // If the cicle is complete return
	} else {
		visited[vertexVisited] = actual;
	}

	edge* actualEdge = actual->edgeList; // Get the edge list from the actual vertex
	int aux;

	while (actualEdge != nullptr) { // While the edge list hasnt finished
		if (!alreadyVisited(visited, vertexVisited, spell.findVertex(actualEdge->to))) { // Stops from going back and forth
			aux = checkBiggestCircle(visited, spell, initial, spell.findVertex(actualEdge->to), count+1, max, vertexVisited+1); // Recursive call
			// If the count of the recursive call is larger than current count replace it
			if(max < aux) max = aux;
		}
		actualEdge = actualEdge->next; // Go to the next edge in the list for the actual vertex
	}
	if (count > max) max = count; // If the current largest count is larger than the current max replace it
	
	return max;
}


int getBiggestCicle(graph spell) {
	vertex* visited[spell.vertexQuantity]; // Array to avoid repeating paths
	return checkBiggestCircle(visited, spell, spell.vertices, spell.vertices, 0, 0);
}


int checkBiggestPath(vertex* visited[], graph spell, vertex* actual, int count, int max, int vertexVisited = 0) {
	if (actual->type != 'A' || actual->type != 'B') {
		if (count > max) max = count;
	}
	
	visited[vertexVisited] = actual; // Mark current vertex as already visited
	
	edge* actEdge = actual->edgeList; // Get the edge list from the actual vertex
	int aux;
	
	while (actEdge != nullptr) { // While the edge list hasnt finished
		if (!alreadyVisited(visited, vertexVisited, spell.findVertex(actEdge->to))) { // Stops from going back and forth
			count += actEdge->weight;
			aux = checkBiggestPath(visited, spell, spell.findVertex(actEdge->to), count, max, vertexVisited+1); // Recursive call
			// If the count of the recursive call is larger than current count replace it
			if (max < aux) max = aux;
			count-= actEdge->weight;
		}
		actEdge = actEdge->next; // Go to the next edge in the list for the actual vertex
	}
	
	visited[vertexVisited] = nullptr; // Desmark current vertex as already visited

	return max;
}


int getBiggestPath(graph spell) {
	int max = 0, aux = 0;
	vertex* visited[spell.vertexQuantity]; // Array to avoid repeating
	vertex* actualRune = spell.vertices;

	for(int i = 0; i < spell.vertexQuantity; i++) { // In case an ilegal spell has more than one confluence points
		if(actualRune->type == 'A') {
			aux = checkBiggestPath(visited, spell, actualRune, 0, 0);
			if(aux > max) max = aux;
		}
		actualRune = actualRune->next;
	}
	return max;
}


void checkSpellLegality(graph spells[], int spellIterator, int &cofluencyCounter, int &elementalsCounter, bool &isEnergeticRune, bool &isCatalidicRune) {
	vertex* actualRune = spells[spellIterator].vertices;
	int lastElementalCounter = elementalsCounter;
	string type;

	for (int runeIterator = 0; runeIterator < spells[spellIterator].vertexQuantity; runeIterator++) {

		switch (actualRune->type) {
			case 'A': cofluencyCounter++; break;
			case 'I': elementalsCounter++; type = "Ignatum"; break;
			case 'Q': elementalsCounter++; type = "Aquos"; break;
			case 'T': elementalsCounter++; type = "Terraminium"; break;
			case 'V': elementalsCounter++; type = "Ventus"; break;
			case 'L': elementalsCounter++; type = "Lux"; break;
			case 'O': elementalsCounter++; type = "Tenebrae"; break;
			case 'F': break;
			case 'B': break;
			case 'D': break;
			default: cout << "\n\e[0;31mSomething really bad happened, check your elementals runes...\e[0m\n"; break;
		}

		if (lastElementalCounter < elementalsCounter) {
			string adjacentTypes = spells[spellIterator].getAdjacentTypes(actualRune);
			for (int adjacentIterator = 0; adjacentIterator < adjacentTypes.length(); adjacentIterator++) {
				if (adjacentTypes[spellIterator] == 'D') isCatalidicRune = false;
			}
		}

		if (actualRune->type == 'A') {
			string adjacentTypes = spells[spellIterator].getAdjacentTypes(actualRune);
			for (int adjacentIterator = 0; adjacentIterator < adjacentTypes.length(); adjacentIterator++) {
				if (adjacentTypes[adjacentIterator] != 'B') isEnergeticRune = false;
			}
		}

		if (actualRune->next != nullptr) actualRune = actualRune->next;
	}

	if (elementalsCounter > 0) spells[spellIterator].type = type;
}


void getIlegalMagicians(graph spells[], magician* &suspects, int spellsQuantity, int &suspectsQuantity) {

	for (int spellIterator = 0; spellIterator < spellsQuantity; spellIterator++) {
		int cofluencyCounter = 0, elementalsCounter = 0;
		bool isEnergeticRune = true, isCatalidicRune = true;

		string magicianName = spells[spellIterator].name;

		checkSpellLegality(spells, spellIterator, cofluencyCounter, elementalsCounter, isEnergeticRune, isCatalidicRune);

		for (int suspectIterator = 0; suspectIterator < suspectsQuantity; suspectIterator++) {

			if (suspects[suspectIterator].name != magicianName) {
				if (suspectIterator == suspectsQuantity - 1) {
					addSuspect(suspects, suspectsQuantity, magicianName);
				}
				continue;
			}

			if (cofluencyCounter > MAX_COFLUENCY || cofluencyCounter == 0) {
				suspects[suspectIterator].ilegalSpells++;
				spells[spellIterator].vality = false;
				printf("\n\e[0;31mERROR: [%s] Cofluency counter exceeded the limit\e[0m\n", magicianName.c_str());
				continue;
			}

			if (elementalsCounter > MAX_ELEMENTALS) {
				suspects[suspectIterator].ilegalSpells++;
				spells[spellIterator].vality = false;
				printf("\n\e[0;31mERROR: [%s] Elemental runes counter exceeded the limit\e[0m\n", magicianName.c_str());
				continue;
			}

			if (!isEnergeticRune) {
				suspects[suspectIterator].ilegalSpells++;
				spells[spellIterator].vality = false;
				printf("\n\e[0;31mERROR: [%s] Energetic runes counter exceeded the limit\e[0m\n", magicianName.c_str());
				continue;
			}

			if (!isCatalidicRune) {
				suspects[suspectIterator].ilegalSpells++;
				spells[spellIterator].vality = false;
				printf("\n\e[0;31mERROR: [%s] Catalidic runes counter exceeded the limit\e[0m\n", magicianName.c_str());
				continue;
			}

			if (!((getBiggestCicle(spells[spellIterator])%2)==0)) {
				suspects[suspectIterator].ilegalSpells++;
				spells[spellIterator].vality = false;
				printf("\n\e[0;31mERROR: [%s] Spell largest cicle is not even\e[0m\n", magicianName.c_str());
				continue;
			}
		}
	}
}


string obtainLastName(string name) {
	string lastName;
	int space = name.find(' '); // Finds the location of the first space in the string
	lastName = name.substr(space + 1); // Gets magcian last name

	return lastName;
}


void nameSpell(graph spell, string &spellName, string magicianName) {
	if (spell.type != "Arcano") {
		spellName += spell.type + " "; // If spell is not arcane then add type as first word
	}

	string lastName = obtainLastName(magicianName); // Gets magician last name
	char lastLetter = lastName.back(); // Gets last letter of the last name

	lastName.pop_back(); // Removes last letter from the last name

	if (lastLetter == 'a' || lastLetter == 'e' || lastLetter == 'i' || lastLetter == 'o' || lastLetter == 'u') {
		lastName += "ium "; // If the last letter is a vocal add "ium" at the end
	} else {
		lastName += "um "; // If the last letter is a consonant add "um" at the end
	}

	spellName += lastName; // Add the modified last name to the spell name

	int bigPath = getBiggestPath(spell); // Get largest path (By ponderation) of the spell
	int bigCicle = getBiggestCicle(spell); // Get largest cicle (By edge number) of the spell

	if (bigPath == 0 || bigCicle == 0) {
		spellName += "Arcante"; // If spell doesnt have either largest path or cicle
	} else {
		if (bigCicle < bigPath) {
			spellName += "modicum";
		} else {
			spellName += "maximus";
		}
	}
}


void writeProcessed(ofstream &file, graph spells[], int spellsQuantity) {
	processedList legal;
	processedList illegal;
	int n;
	
	file << "Hechizos Legales" << endl << endl;

	for (int spellIterator = 0; spellIterator < spellsQuantity; spellIterator++) {
		if (spells[spellIterator].vality) { // If spell is legal
			string magicianName = spells[spellIterator].name; // Get magician name

			nameSpell(spells[spellIterator], spells[spellIterator].spellName, magicianName); // Get spell correct name
			
			legal.insertInOrder(spells[spellIterator]); // Insert spells in order onto the list
		}
	}

	n = legal.size();

	for (int i = 0; i < n; i++) {
		file << legal.begin()->spellName << endl // Writes spell name
			 << legal.begin()->name<< endl << endl; // Writes magician name

		legal.erase(legal.begin());
	}
	
	file << "Hechizos Ilegales" << endl;
	for (int spellIterator = 0; spellIterator < spellsQuantity; spellIterator++) {
		if(!spells[spellIterator].vality) { // If spell is illegal
			string magicianName = spells[spellIterator].name; // Get magician name

			nameSpell(spells[spellIterator], spells[spellIterator].spellName, magicianName); // Get spell correct name
			
			illegal.insertInOrder(spells[spellIterator]); // Insert spells in order onto the list
		}
	}

	n = illegal.size();

	for (int i = 0; i < n; i++) {
		file << illegal.begin()->spellName << endl // Writes spell name
			 <<illegal.begin()->name << endl << endl; // Writes magician name
		
		illegal.erase(legal.begin());
	}
}


void writeSuspects(ofstream &file, magician suspects[], int suspectQuantity) {
	for (int i = 0; i < suspectQuantity; i++) {
		if (suspects[i].ilegalSpells > 2) {
			file << suspects[i].name << endl;
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
	
	spellsFile.close();
	suspectsFile.close();

	// Check if the spells are valid duh
	getIlegalMagicians(spells, suspects, spellsQuantity, suspectsQuantity);

	// Create the processed spells file
	ofstream processedSpells("../build/processedSpells.out");
	writeProcessed(processedSpells, spells, spellsQuantity);

	// Update the suspects list
	ofstream investigationFile("../build/underInvestigation.in");
	writeSuspects(investigationFile, suspects, suspectsQuantity);

	// Just debug stuff
	printGraphs(spells, spellsQuantity);
	printSuspectThings(suspects, suspectsQuantity);

	investigationFile.close();
	processedSpells.close();

	return 0;
}
