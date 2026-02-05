#include "../includes/shader.hpp"
#include "../includes/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

Shader::Shader() {
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(vertexShader, 1, &this->vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &this->fragmentShaderSource, NULL);
    
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLint success;
    GLchar infoLog[512];
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed:\n" << infoLog << std::endl;
	}
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed:\n" << infoLog << std::endl;
	}

    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);
    
    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	this->lastDeltaTimeForChange = 0.0f;
	this->currentDisplayMode = DisplayMode::FILL;
	this->color = {1.0f, 1.0f, 1.0f};
	this->incrementColorValues = false;
	this->breatheEffect = false;
	this->textureOpacity = 0.0f;
	this->showTexture = false;
	this->finishedApplyTexture = true;
}

Shader::~Shader() {
	glDeleteProgram(this->shaderProgram);
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

void Shader::setUniform1f(const char* name, float value) {
	glUniform1f(glGetUniformLocation(this->shaderProgram, name), value);
}

void Shader::setUniform3f(const char* name, float* value) {
	glUniform3f(glGetUniformLocation(this->shaderProgram, name), value[0], value[1], value[2]);
}

void Shader::setColor(float *newColor) {
	if (this->breatheEffect == true)
		this->breatheEffect = false;
    this->color[0] = newColor[0];
    this->color[1] = newColor[1];
    this->color[2] = newColor[2];
}

float *Shader::getColor() {
	return this->color.data();
}

void Shader::incrementColor() {
	if (this->color[0] > 1.0f)
	{
		for (int i = 0; i < 3; i++)
			this->color[i] = 1.0f;
		this->incrementColorValues = false;
	}
	else
	{
		for (int i = 0; i < 3; i++)
			this->color[i] += 0.01f;
	}
}

void Shader::decrementColor() {
	if (this->color[0] < 0.0f)
	{
		for (int i = 0; i < 3; i++)
			this->color[i] = 0.0f;
		this->incrementColorValues = true;
	}
	else
	{
		for (int i = 0; i < 3; i++)
			this->color[i] -= 0.01f;
	}
}

bool Shader::getIncrementColorValues() {
	return this->incrementColorValues;
}

bool Shader::getBreatheEffect() {
	return this->breatheEffect;
}

void Shader::setBreatheEffect(bool newValue) {
	if (this->lastDeltaTimeForChange + 0.5f < glfwGetTime())
	{
		this->breatheEffect = newValue;
		this->lastDeltaTimeForChange = glfwGetTime();
	}
}

bool Shader::getFinishedApplyTexture() {
	return this->finishedApplyTexture;
}

void Shader::setFinishedApplyTexture(bool newValue) {
	this->finishedApplyTexture = newValue;
}

void Shader::incrementTextureOpacity(float deltaTime) {
    float speed = 2.0f;
    this->textureOpacity += speed * deltaTime;
    if (this->textureOpacity > 1.0f)
	{
		this->finishedApplyTexture = true;
        this->textureOpacity = 1.0f;
	}
}

void Shader::decrementTextureOpacity(float deltaTime) {
    float speed = 2.0f;
    this->textureOpacity -= speed * deltaTime;
    if (this->textureOpacity < 0.0f)
	{
		this->finishedApplyTexture = true;
        this->textureOpacity = 0.0f;
	}
}

float Shader::getTextureOpacity() {
	return this->textureOpacity;
}

bool Shader::getShowTexture() {
	return this->showTexture;
}

void Shader::setShowTexture(bool newValue) {
	this->showTexture = newValue;
}

void Shader::changeMode() {
	float currentTime = glfwGetTime();

	if (this->lastDeltaTimeForChange + 0.5f < currentTime)
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
