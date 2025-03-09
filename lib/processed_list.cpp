#include "list.cpp"
#include "graph.cpp"

#pragma once

class processedList: public list<graph>{
	public:
		iterator arcane;
		iterator fire;
		iterator water;
		iterator earth;
		iterator wind;
		iterator light;

		void insertInOrder(graph spell) {
			string type = spell.type;

			if (type == "Arcano") {
				arcane = insert(arcane, spell);

			} else if (type == "Ignatum") {
				if (fire == nullptr) fire == arcane;
				fire = insert(fire, spell);

			} else if (type == "Aquos") {
				if (water == nullptr) water  ==  fire;
				water = insert(water, spell);

			} else if (type == "Terraminium") {
				if (earth == nullptr) earth=water;
				earth = insert(earth, spell);

			} else if (type == "Ventus") {
				if (wind == nullptr) wind=earth;
				wind = insert(wind, spell);

			} else if (type == "Lux") {
				if (light == nullptr) light=wind;
				light = insert(light, spell);

			} else {
				push_back(spell);
			}
		}
};
