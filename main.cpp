#include "GrimmMap.h"
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

int main() {
    auto map = std::make_unique<GrimmMap>("fairytales/map.json");
    vector<string> destinations = map->first_move();
    string selection;
    while (!map->is_done()) {
	std::cout << "\nwhere would you like to go?\n";
	for (auto &dest: destinations)
	    std::cout << "\t- " << dest << std::endl;

	std::cin >> selection;
	while (std::find(destinations.begin(), destinations.end(), selection) == destinations.end()) {
	    std::cerr << "invalid selection\n";
	    std::cin >> selection;
	}

	destinations = map->go_to(selection);
    }
    std::cout << "THE END\n";
    return 0;
}
