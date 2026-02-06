#ifndef SHADER_HPP
# define SHADER_HPP

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>

enum DisplayMode {
	POINT = GL_POINT,
	LINE = GL_LINE,
	FILL = GL_FILL,
};

class Shader
{
private:
	unsigned int shaderProgram;
	DisplayMode currentDisplayMode;
	
	float lastDeltaTimeForChange;

	std::vector<float> color = {};
	bool incrementColorValues;
	bool breatheEffect;

	float textureOpacity;
	bool showTexture;
	bool finishedApplyTexture;

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexture;\n"
		"layout (location = 2) in vec3 faceColor;\n"
		"\n"
		"uniform vec3 aColor;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 perspective;\n"
		"uniform vec3 centerPos;\n"
		"uniform int rotationBool;\n"
		"uniform int colorPerFace;\n"
		"\n"
		"out vec2 textureCoords;\n"
		"out vec3 fragColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	vec4 tmpPos;\n"
		"	vec3 outputColor;\n"
		"\n"
		"	if (rotationBool == 1) {\n"
		"		tmpPos = vec4(aPos, 1) - vec4(centerPos, 1);\n"
		"		tmpPos = tmpPos * model;\n"
		"		tmpPos = tmpPos + vec4(centerPos, 1);\n"
		"	} else {\n"
		"		tmpPos = model * vec4(aPos, 1);\n"
		"	}\n"
		"\n"
		"	if (colorPerFace == 1) {\n"
		"		outputColor = faceColor;\n"
		"	} else {\n"
		"		outputColor = aPos * aColor;\n"
		"	}\n"
		"\n"
		"	gl_Position = perspective * view * tmpPos;\n"
		"	textureCoords = aTexture;\n"
		"	fragColor = outputColor;\n"
		"}\0";

	const char *fragmentShaderSource = "#version 330 core\n"
		"in vec3 fragColor;\n"
		"in vec2 textureCoords;\n"
		"\n"
		"out vec4 outColor;\n"
		"\n"
		"uniform sampler2D ourTexture;\n"
		"uniform float textureOpacity;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	vec4 color = vec4(fragColor, 1.0);\n"
		"	vec4 tex = texture(ourTexture, textureCoords);\n"
		"	outColor = mix(color, tex, textureOpacity);\n"
		"}\0";

public:
	Shader();
	~Shader();

	void useProgram();
	void setUniformMatrix4x4(const float *matrix, const char *name);
	void setUniformMatrix3x3(const float *matrix, const char *name);
	void setUniform1i(const char* name, int value);
	void setUniform1f(const char* name, float value);
	void setUniform3f(const char* name, float* value);

	void setColor(float *newColor);
	float* getColor();
	void incrementColor();
	void decrementColor();
	bool getIncrementColorValues();
	bool getBreatheEffect();
	void setBreatheEffect(bool newValue);

	bool getFinishedApplyTexture();
	void setFinishedApplyTexture(bool newValue);
	void incrementTextureOpacity(float deltaTime);
	void decrementTextureOpacity(float deltaTime);
	float getTextureOpacity();
	bool getShowTexture();
	void setShowTexture(bool newValue);


	void changeMode();
	int getMode();
};

#endif

