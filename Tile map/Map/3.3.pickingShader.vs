#version 330 core

uniform mat4 transform;
uniform mat4 terrainTransform;
uniform vec4 colour;

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
  
out vec4 ourColor; // output a color to the fragment shader

void main()
{

	gl_Position = terrainTransform * transform * vec4(aPos, 1.0);

	ourColor = colour;

} 