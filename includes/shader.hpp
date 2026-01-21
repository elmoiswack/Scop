#ifndef SHADER_CPP
# define SHADER_CPP

#include <iostream>

class Shader
{
private:
	unsigned int shaderProgram;
	
	const char *vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexture;\n"
		"layout (location = 2) in vec3 aNormal;\n"
		"\n"
		"uniform mat4 view;\n"
		"uniform mat4 perspective;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = perspective * view * vec4(aPos, 1);\n"
		"}\0";

	const char *fragmentShaderSource = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		 "	FragColor = vec4(0.5, 0.5, 0.5, 1.0);"
		"}\0";

public:
	Shader();
	~Shader();

	unsigned int tmp(std::string);
	void useProgram();
	void setUniformMatrix4x4(const float *matrix, const char *name);
};

#endif