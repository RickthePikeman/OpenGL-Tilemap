#pragma once

class Building {
public:
	Building() {

	}
	virtual int getBuildingID() {
		return 0;
	}
	virtual void sayBuildingType() {
		std::cout << "No building here m'lord" << std::endl;
	}
};