#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in vec3 faceColor;
		
uniform vec3 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform vec3 centerPos;
uniform int rotationBool;
uniform int colorType;
		
out vec2 textureCoords;
out vec3 fragColor;
		
void main()
{
	vec4 tmpPos;
		
	if (rotationBool == 1) {
		tmpPos = vec4(aPos, 1) - vec4(centerPos, 1);
		tmpPos = tmpPos * model;
		tmpPos = tmpPos + vec4(centerPos, 1);
	} else {
		tmpPos = model * vec4(aPos, 1);
	}
		
	if (colorType == 0) {
		fragColor = faceColor;
	} else if (colorType == 1) { 
		fragColor = abs(normalize(aPos)) * aColor;
	} else {
		fragColor = aColor;
	}
		
	gl_Position = perspective * view * tmpPos;
	textureCoords = aTexture;
};