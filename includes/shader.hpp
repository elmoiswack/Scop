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

	float colorOpacity;
	bool incrementColorValue;
	bool breatheEffect;

	float textureOpacity;
	bool showTexture;
	bool finishedApplyTexture;

	char* vertexShaderSource;
	char* fragmentShaderSource;

	

	

public:
	Shader();
	~Shader();

	char* readFromFile(const char* pathToFile);
	void useProgram();
	void setUniformMatrix4x4(const float *matrix, const char *name);
	void setUniformMatrix3x3(const float *matrix, const char *name);
	void setUniform1i(const char* name, int value);
	void setUniform1f(const char* name, float value);
	void setUniform3f(const char* name, float* value);

	void incrementColorOpacity();
	void decrementColorOpacity();
	float getColorOpacity();
	bool getIncrementColorValue();
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

	class CompilingShaderException : public std::exception {
	public:
		const char *what() const throw() {
			return ("Failed to compile shaders!");
		}
	};
};

#endif

