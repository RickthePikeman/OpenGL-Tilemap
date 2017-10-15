#pragma once
#include <glm\glm.hpp>
#include "TileCoordinates.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Building.h"
#include "House.h"

class Tile {
private:
	int textureID = 0;//Is is better to just have an ID for what texture the tile is meant to have 
	//OR is it better for the tile to just have its own copy of the texture it wants to use?
	Building *construction = nullptr;
public:
	TileCoordinates coordinates;
	Tile(void) {
		coordinates = TileCoordinates(1, 1);
	}
	glm::mat4 GetPos(void) {
		glm::mat4 transform = glm::mat4(1.0);
		transform = glm::translate(transform, glm::vec3(coordinates.X(), coordinates.Y(), 0.0f));
		return transform;
	}
	int GetTextureID() {
		return textureID;
	}
	void SetTextureID(int textureID) {
		this->textureID = textureID;
	}
	void makeBuilding(int type) {
		if (type < 0) {
			delete construction;
			construction = nullptr;
		}
		else if (type == 1) {
			delete construction;
			construction = new House(1);
		}
	}
	int getBuildingType() {
		if (construction == nullptr) {
			return 0;
		}
		return construction->getBuildingID();
	}
	void sayBuildingType() {
		if (construction == nullptr) {
			std::cout << "No building here m'lord" << std::endl;
			return;
		}
		construction->sayBuildingType();
	}
};

