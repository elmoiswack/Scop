#ifndef SHADER_HPP
# define SHADER_HPP

#include <iostream>

class Shader
{
private:
	unsigned int shaderProgram;
	
	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexture;\n"
		"layout (location = 2) in vec3 aNormal;\n"
		"\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 perspective;\n"
		"\n"
		"out vec2 textureColor;\n"
		"out vec3 Normal;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = perspective * view * model * vec4(aPos, 1);\n"
		"	textureColor = aTexture;\n"
		"	Normal = aNormal;\n"
		"}\0";

	// const char *fragmentShaderSource = "#version 330 core\n"
	// 	"in vec2 textureCoords;\n"
	// 	"in vec3 Normal;\n"
	// 	"\n"
	// 	"in vec3 Ka;\n"
	// 	"in vec3 Ks;\n"
	// 	"in vec3 Kd;\n"
	// 	"\n"
	// 	"in float Ns;\n"
	// 	"in float Ni;\n"
	// 	"in float d;\n"
	// 	"in int illum;\n"
	// 	"\n"
	// 	"out vec4 color;\n"
	// 	"\n"
	// 	"uniform sampler2D text;\n"
	// 	"\n"
	// 	"void main()\n"
	// 	"{\n"
	// 	"	vec3 norm = normalize(Normal);\n"
	// 	"	"
	// 	"	color = texture(tex, textureCoords);\n"
	// 	"}\0";

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

