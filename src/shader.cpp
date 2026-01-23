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
}

Shader::~Shader()
{
	glDeleteShader(this->shaderProgram);
}

void Shader::useProgram() {
	glUseProgram(this->shaderProgram);
}

unsigned int Shader::tmp(std::string tmp) {
	return glGetUniformLocation(this->shaderProgram, tmp.c_str());
}

void Shader::setUniformMatrix4x4(const float *matrix, const char *name) {
	glUniformMatrix4fv(
		glGetUniformLocation(this->shaderProgram, name),
		1, GL_FALSE, matrix
	);
}

void Shader::setUniform1i(const char* name, int value) {
	glUniform1i(glGetUniformLocation(this->shaderProgram, name), value);
}