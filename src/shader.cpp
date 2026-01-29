#include "../includes/shader.hpp"
#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

Shader::Shader()
{
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	this->shaderProgram = glCreateProgram();

	glShaderSource(vertexShader, 1, &this->vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &this->fragmentShaderSource, NULL);
	
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	glAttachShader(this->shaderProgram, vertexShader);
	glAttachShader(this->shaderProgram, fragmentShader);
	glLinkProgram(this->shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	this->lastDeltaTimeForChange = 0.0f;
	this->currentDisplayMode = DisplayMode::FILL;
}

Shader::~Shader()
{
	glDeleteShader(this->shaderProgram);
}

void Shader::useProgram() {
	glUseProgram(this->shaderProgram);
}

void Shader::setUniformMatrix4x4(const float *matrix, const char *name) {
	glUniformMatrix4fv(
		glGetUniformLocation(this->shaderProgram, name),
		1, GL_FALSE, matrix
	);
}

void Shader::setUniformMatrix3x3(const float *matrix, const char *name) {
	glUniformMatrix3fv(
		glGetUniformLocation(this->shaderProgram, name),
		1, GL_FALSE, matrix
	);
}

void Shader::setUniform1i(const char* name, int value) {
	glUniform1i(glGetUniformLocation(this->shaderProgram, name), value);
}

void Shader::setUniform3f(const char* name, float* value) {
	glUniform3f(glGetUniformLocation(this->shaderProgram, name), value[0], value[1], value[2]);
}


void Shader::changeMode() {
	float currentTime = glfwGetTime();

	if (this->lastDeltaTimeForChange + 1.0f < currentTime)
	{
		this->lastDeltaTimeForChange = currentTime;
		switch (this->currentDisplayMode)
		{
		case DisplayMode::POINT:
			this->currentDisplayMode = DisplayMode::LINE;
			break ;
		case DisplayMode::LINE:
			this->currentDisplayMode = DisplayMode::FILL;
			break ;
		case DisplayMode::FILL:
			this->currentDisplayMode = DisplayMode::POINT;
			break ;
		default:
			this->currentDisplayMode = DisplayMode::FILL;
			break;
		}
	}
}

int Shader::getMode() {
	return (this->currentDisplayMode);
}
