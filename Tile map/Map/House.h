#pragma once
#include "Building.h"
#include <iostream>

class House : public Building {
public:
	House(int i) {

	}
	int getBuildingID() {
		return 1;
	}
	void sayBuildingType() {
		std::cout << "Theres a house here m'lord" << std::endl;
	}
};