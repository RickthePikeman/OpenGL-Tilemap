#version 330 core

uniform mat4 transform;
uniform mat4 terrainTransform;
uniform vec4 colour;

layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
layout (location = 2) in vec2 aTex;
  
out vec4 ourColour; // output a color to the fragment shader
out vec2 textureCoord;

void main()
{


	gl_Position = terrainTransform * transform * vec4(aPos, 0.0, 1.0);
	textureCoord = aTex;

	ourColour = colour;

} 