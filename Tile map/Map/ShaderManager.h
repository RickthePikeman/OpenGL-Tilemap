#pragma once
#include "shader_setup.h"
#include <iostream>

static class ShaderManager {
private:
	unsigned int shader;
	unsigned int transformLoc;
	unsigned int colourLoc;
	unsigned int sceneTransformLoc;
	
public:
	ShaderManager( std::string shaderName) {
		shader = setupShaders(std::string("3.3." + shaderName + ".vs"), std::string("3.3." + shaderName + ".fs"));
		transformLoc = glGetUniformLocation(shader, "transform");
		colourLoc = glGetUniformLocation(shader, "colour");
		sceneTransformLoc = glGetUniformLocation(shader, "terrainTransform");

	}
	unsigned int Shader() {
		return shader;
	}
	unsigned int Transform() {
		return transformLoc;
	}
	unsigned int Colour() {
		return colourLoc;
	}
	unsigned int SceneTransform() {
		return sceneTransformLoc;
	}
};