#pragma once
#include <vector>

static class TileMetrics {
	
public:
	const float WIDTH = 0.1, HEIGHT = 0.05;

	std::vector <float> corners = {
		-WIDTH, 0.0f,//Left
		0.0f, HEIGHT,//Top
		WIDTH, 0.0f,//Right
		0.0f, -HEIGHT};//Down
};

