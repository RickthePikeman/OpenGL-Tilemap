#version 330 core

uniform sampler2D textureImage;
in vec4 ourColour;
in vec2 textureCoord;


out vec4 FragColour;  
  
void main()
{
	FragColour = texture2D(textureImage,textureCoord);
}