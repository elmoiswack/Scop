#ifndef SHADER_HPP
# define SHADER_HPP

#include <iostream>

class Shader
{
private:
	unsigned int shaderProgram;
	
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 perspective;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = perspective * view * model * vec4(aPos, 1);\n"
		"}\0";

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(0.7, 0.2, 0.3, 1.0);\n"
		"}\0";

public:
	Shader();
	~Shader();

	unsigned int tmp(std::string);
	void useProgram();
	void setUniformMatrix4x4(const float *matrix, const char *name);
	void setUniform1i(const char* name, int value);
};

#endif

//"	color = texture(tex, textureCoord);\n"