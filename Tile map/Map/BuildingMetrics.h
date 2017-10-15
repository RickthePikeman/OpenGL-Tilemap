#pragma once
#include <vector>

static class BuildingMetrics {

public:
	const float WIDTH = 0.1, HEIGHT = 0.1, displacement = -0.05;

	std::vector <float> corners = {
		//-WIDTH, -HEIGHT,//LowerLeft
		//-WIDTH, +HEIGHT,//UpperLeft
		//0.0f, HEIGHT + HEIGHT/2,//Top
		//WIDTH, +HEIGHT,//UpperLeft
		//WIDTH, -HEIGHT,//LowerRight
		//0.0f, -HEIGHT - HEIGHT / 2 };//Down

		-WIDTH, -HEIGHT/2- displacement,//LowerLeft
		-WIDTH, +HEIGHT/2 - displacement,//UpperLeft
		0.0f, HEIGHT - displacement,//Top
		WIDTH, +HEIGHT/2 - displacement,//UpperLeft
		WIDTH, -HEIGHT/2 - displacement,//LowerRight
		0.0f, -HEIGHT - displacement
};//Down

		//-WIDTH, 0.0f,//Left
		//0.0f, HEIGHT,//Top
		//WIDTH, 0.0f,//Right
		//0.0f, -HEIGHT };//Down

	//0.0f, 0.5f,//Left
	//0.5f, 0.0f,//Down
	//1.0f, 0.5f,//Right
	//0.5f, 1.0f//Top

	//0.0f, 1.5f,//UpperLeft
	//0.0f, 0.5f,//LowerLeft
	//0.5f, 0.0f,//Down
	//1.0f, 0.5f,//LowerRight
	//1.0f, 1.5f,//UpperRight
	//0.5f, 1.0f//Top
};