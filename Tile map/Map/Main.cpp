#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <ctime>

#include <math.h>
#include <vector>

#include <glew\glew.h>
#include <GLFW/glfw3.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture_loader.h"
#include "HighLighter.h"
#include "MapManager.h"//includes: TileMetrics, Tile & TileCoordinates
#include "ShaderManager.h"

using namespace std;

void processInput(GLFWwindow *window, HighLighter*, MapManager*);
void RenderPickingColours(MapManager*, ShaderManager*);
void RenderTiles(MapManager*, ShaderManager*);
void RenderBuildings(MapManager*, ShaderManager*);
TileCoordinates GetTilePostion(GLFWwindow*);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

static int GRIDSIZE;//This is set by the map manager
//This might get removed completely later to just use the object calls. It just feels like it would be called to much, so it warranted its own copy. Problem is that this isnt a const 


int main()
{
#pragma region Initialise libraries and Window
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tile Map", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeLimits(window, SCR_WIDTH, SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT);//The window now cant be resized since the size can only go from 800,600 to 800,600

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, (double)SCR_WIDTH, 0.0, (double)SCR_HEIGHT, 0.0, 1.0); // this creates a canvas you can do 2D drawing on

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion

	ShaderManager pickingShader("pickingShader");

	ShaderManager textureShader("textureShader");

	MapManager tileMap(&GRIDSIZE);
	
	HighLighter tileSelecter(SCR_WIDTH,SCR_HEIGHT);

	glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

	while (!glfwWindowShouldClose(window))// render loop
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 terrainMatrix = glm::mat4(1.0);//Will be used later for moving the camera

		glUseProgram(pickingShader.Shader());
		glUniformMatrix4fv(pickingShader.SceneTransform(), 1, GL_FALSE, (GLfloat*)&terrainMatrix);
		RenderPickingColours(&tileMap, &pickingShader);
		
		processInput(window, &tileSelecter, &tileMap);

		glUseProgram(textureShader.Shader());
		glUniformMatrix4fv(textureShader.SceneTransform(), 1, GL_FALSE, (GLfloat*)&terrainMatrix);
		RenderTiles(&tileMap, &textureShader);

		tileSelecter.DrawHighLighter();//This will be using shaders soon

		glUseProgram(textureShader.Shader());

		RenderBuildings(&tileMap, &textureShader);//This building render function should be merged with tile render function.
		//Problem is, is that the tile selecter would be rendered over the building and wouldnt look to nice
		//Need a solution to this

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window, HighLighter *tileSelecter, MapManager *tileMap)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	TileCoordinates selectedTile = GetTilePostion(window);//This get the mouses postion and find the tile (if any) underneath it
	if (selectedTile.X() > -1) {

		tileSelecter->setHighlighterPostion(tileMap->GetTileMap()[(int)selectedTile.X()][(int)selectedTile.Y()].coordinates.X(), tileMap->GetTileMap()[(int)selectedTile.X()][(int)selectedTile.Y()].coordinates.Y());
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		if (selectedTile.X() > -1) {
			tileMap->sayBuildingType((int)selectedTile.X(), (int)selectedTile.Y());
		}
	}
	
}

void RenderPickingColours(MapManager *tileMap, ShaderManager *pickingShader){
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int k = 0; k < GRIDSIZE; k++) {

			int r = (((i + 1) + (k ) * GRIDSIZE) & 0x000000FF) >> 0;//I is incremented so pure black isnt used as an ID for the tile
			int g = (((i + 1) + (k ) * GRIDSIZE) & 0x0000FF00) >> 8;//This is because the the background is black. 
			int b = (((i + 1) + (k ) * GRIDSIZE) & 0x00FF0000) >> 16;//Who knows we might want to be able to select the background at some point

			glUniform4f(pickingShader->Colour(), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);//The colour of the tile is used as the tiles ID

			glUniformMatrix4fv(pickingShader->Transform(), 1, GL_FALSE, (GLfloat*)&(tileMap->GetTileMap()[i][k].GetPos()));
			glBindVertexArray(tileMap->GetTileVAO());
			glDrawArrays(GL_TRIANGLE_FAN, 0, TileMetrics().corners.size() / 2);//2 = (x,y) coordinates
		}
	}
}

void RenderTiles(MapManager *tileMap, ShaderManager *textureShader) {
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int k = 0; k < GRIDSIZE; k++) {

			glUniformMatrix4fv(textureShader->Transform(), 1, GL_FALSE, (GLfloat*)&(tileMap->GetTileMap()[i][k].GetPos()));


			glBindTexture(GL_TEXTURE_2D, tileMap->GetTileTexture(i,k));

			glBindVertexArray(tileMap->GetTileVAO());
			glDrawArrays(GL_TRIANGLE_FAN, 0, TileMetrics().corners.size() / 2);//2 = (x,y) coordinates
		}
	}
}

void RenderBuildings(MapManager *tileMap, ShaderManager *textureShader) {
	for (int i = GRIDSIZE-1; i > -1; i--) {//We want the buildings to be rendered from back to front. Otherwise we get overlap
		for (int k = GRIDSIZE-1; k > -1; k--) {
			if (tileMap->GetTileMap()[i][k].getBuildingType() > 0) {
				glUniformMatrix4fv(textureShader->Transform(), 1, GL_FALSE, (GLfloat*)&(tileMap->GetTileMap()[i][k].GetPos()));
				glBindTexture(GL_TEXTURE_2D, tileMap->GetBuildingTexture(i,k));
				glBindVertexArray(tileMap->GetBuildingVAO());
				glDrawArrays(GL_TRIANGLE_FAN, 0, BuildingMetrics().corners.size() / 2);//2 = (x,y) coordinates
			}
		}
	}
}

TileCoordinates GetTilePostion(GLFWwindow* window) {
	unsigned char data[4];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);

	int pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;

	if (pickedID == 0x00000000){ // black has been picked, must be the background !//0x00ffffff = Full white
	}
	else {//Means a tile has been detected since its found a colour (or is some cases the area outside of the window)
		int tileID = pickedID-1;
		if (tileID > (GRIDSIZE*GRIDSIZE)) {//This is to stop using pickedID which can be picked from outside the window
			return TileCoordinates(-1, -1);
		}
		int tileX = tileID - ((tileID / GRIDSIZE) * GRIDSIZE), tileY = tileID / GRIDSIZE;
		return TileCoordinates(tileX, tileY);//Returns their postion in the tileMap array. NOT based on their postion in the screen
	}
	return TileCoordinates(-1, -1);
}