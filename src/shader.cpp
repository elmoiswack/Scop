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

	this->currentDrawingMode = 0;
	this->lastDeltaTimeForChange = 0.0f;
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
	if (glfwGetTime() > (this->lastDeltaTimeForChange + 1.0f)) {
		this->lastDeltaTimeForChange = glfwGetTime();
		if (this->currentDrawingMode == 2)
			this->currentDrawingMode = 0;
		else
			this->currentDrawingMode += 1;
	}
}

int Shader::getMode() {
	return (this->bla[this->currentDrawingMode]);
}
