#pragma once

class TileCoordinates {
	float x, y;
public:
	float X(){
		return x;
	}
	float Y() {
		return y;
	}
	TileCoordinates(float x, float y) {
		this->x = x;
		this->y = y;
	}
	
	TileCoordinates() {
		x = 0;
		y = 0;
	}
};