#pragma once
#include <glew\glew.h>
#include "TileMetrics.h"
#include "BuildingMetrics.h"
#include "Tile.h"
#include "TileCoordinates.h"

class MapManager {
private:
	const static int GRIDSIZE = 10;//The map size is set from here

	unsigned int tileVBO;
	unsigned int tileVAO;
	unsigned int tileTexCoordVBO;

	unsigned int buildingVBO;
	unsigned int buildingVAO;
	unsigned int buildingTexCoordVBO;

	const float tileTextureCoords[8] = {//Note: setting coords going from left, up, right, down causes the tileTextures to be rendered upside down
		0.0f, 0.5f,//Left
		0.5f, 0.0f,//Down
		1.0f, 0.5f,//Right
		0.5f, 1.0f//Top
	};

	const float buildingTextureCoords[12] = {
		0.0f, 0.75f,//UpperLeft
		0.0f, 0.25f,//LowerLeft
		0.5f, 0.0f,//Bottom middle
		1.0f, 0.25f,//LowerRight
		1.0f, 0.75f,//UpperRight
		0.5f, 1.0f//Top middle
	};

	const static int BUILDINGTYPES = 1;
	unsigned int buildingTextures[BUILDINGTYPES];
	const static int TILETYPES = 4;
	unsigned int tileTextures[TILETYPES];

	Tile tileGrid[GRIDSIZE][GRIDSIZE];

	void setTextures() {
		std::string ossToChar;
		for (int i = 0; i < TILETYPES; i++) {
			std::ostringstream oss;
			oss << "Textures\\Grass" << i + 1 << ".png";
			ossToChar = oss.str();
			const char *var = ossToChar.c_str();
			tileTextures[i] = fiLoadTexture(var);
		}
		for (int i = 0; i < BUILDINGTYPES; i++) {
			std::ostringstream oss;
			oss << "Textures\\Building" << i + 1 << ".png";
			ossToChar = oss.str();
			const char *var = ossToChar.c_str();
			buildingTextures[i] = fiLoadTexture(var);
		}
	}
	void setupTiles(const int verticalDisplacement) {
		for (int i = 0; i < GRIDSIZE; i++) {
			for (int k = 0; k < GRIDSIZE; k++) {

				tileGrid[i][k].coordinates = TileCoordinates(((i + verticalDisplacement)*-TileMetrics().WIDTH) + ((k + verticalDisplacement) *TileMetrics().WIDTH), ((k + verticalDisplacement) *TileMetrics().HEIGHT) + ((i + verticalDisplacement) *TileMetrics().HEIGHT));
				int random = rand() % TILETYPES;
				tileGrid[i][k].SetTextureID(random);
				if (random == 3) {
					tileGrid[i][k].makeBuilding(1);
				}
			}
		}
	}
public:
	MapManager(int* GetMapSize) {
		*GetMapSize = GRIDSIZE;

		srand(time(NULL));

#pragma region VBO & VAO setup
		glGenVertexArrays(1, &tileVAO);
		glGenBuffers(1, &tileVBO);
		glBindVertexArray(tileVAO);

		glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(TileMetrics().corners.size()), &TileMetrics().corners[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);


		glGenBuffers(1, &tileTexCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tileTexCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tileTextureCoords), tileTextureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
		glEnableVertexAttribArray(2);


		glGenVertexArrays(1, &buildingVAO);
		glGenBuffers(1, &buildingVBO);
		glBindVertexArray(buildingVAO);

		glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(BuildingMetrics().corners.size()), &BuildingMetrics().corners[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);


		glGenBuffers(1, &buildingTexCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, buildingTexCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buildingTextureCoords), buildingTextureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
		glEnableVertexAttribArray(2);
#pragma endregion

		setTextures();

		setupTiles(-8);
	}

	unsigned int & GetTileVAO() {//Returns the vertex array for the tiles
		return tileVAO;
	}
	unsigned int & GetBuildingVAO() {//Returns the vertex array for the buildings
		return buildingVAO;
	}
	Tile (&GetTileMap())[GRIDSIZE][GRIDSIZE] {//Returns a reference of the 2D Tilemap
		return tileGrid;
	}
	unsigned int GetTileTexture(int x, int y) {
		return tileTextures[tileGrid[x][y].GetTextureID()];
	}
	void setTileTexture(int x, int y, int ID) {
		tileGrid[x][y].SetTextureID(ID);
	}
	unsigned int GetBuildingTexture(int x, int y) {
		return buildingTextures[tileGrid[x][y].getBuildingType()-1];
	}
	
	void sayBuildingType(int x,int y) {
		tileGrid[x][y].sayBuildingType();
	}

	~MapManager() {
		glDeleteVertexArrays(1, &tileVAO);
		glDeleteBuffers(1, &tileVBO);
		glDeleteBuffers(1, &tileTexCoordVBO);
	}
};