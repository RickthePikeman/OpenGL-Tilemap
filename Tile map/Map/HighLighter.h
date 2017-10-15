#pragma once
#include <vector>
#include <glew\glew.h>

class HighLighter {
private:


	/*static float highLighter_vertices[8];*/
	std::vector <float> highLighter_vertices = {
		-0.1f / 2, 0.0f,//Left
		0.0f ,0.05f / 2 ,//Top
		0.1f / 2 ,0.0f ,//Right
		0.0f ,-0.05f / 2 //Down
	};
	unsigned int SCR_WIDTH , SCR_HEIGHT;
	float xPos = -2.0f, yPos = 0.0f;
	float displacement = 0.5;

public:
	HighLighter(const unsigned int& WIDTH, const unsigned int& HEIGHT) {

		SCR_WIDTH = WIDTH;
		SCR_HEIGHT = HEIGHT;

	}
	void DrawHighLighter() {
		glUseProgram(0);
		glPointSize(10);
		glLineWidth(2.5);
		glColor3f(0.9, 0.9, 0.9);
		//glBegin(GL_LINES);

		glBegin(GL_LINE_LOOP);


		for (int i = 0; i < 8; i += 2) {
			glVertex3f((highLighter_vertices[i]  + xPos/2 + displacement) * SCR_WIDTH , (highLighter_vertices[i + 1] + displacement + yPos/2)*SCR_HEIGHT, 0.0);

		}

		glEnd();
	}
	void setHighlighterPostion(float x, float y) {
		xPos = x;
		yPos = y;
	}
};