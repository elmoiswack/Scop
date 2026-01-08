#ifndef SHADER_CPP
# define SHADER_CPP

#include <iostream>

class Shader
{
private:
	unsigned int shaderProgram;
	
	const char *vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec4 aPos;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos);\n"
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
};

#endif