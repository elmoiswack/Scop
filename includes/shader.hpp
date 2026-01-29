#ifndef SHADER_HPP
# define SHADER_HPP

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Shader
{
private:
	unsigned int shaderProgram;
	
	int currentDrawingMode;
	GLenum bla[3] = {GL_POINT, GL_LINE, GL_FILL};
	float lastDeltaTimeForChange;

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexture;\n"
		"layout (location = 2) in vec3 aNormal;\n"
		"\n"
		"uniform vec3 aColor;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 perspective;\n"
		"\n"
		"out vec2 textureColor;\n"
		"out vec3 Normal;\n"
		"out vec3 fragColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = perspective * view * model * vec4(aPos, 1);\n"
		"	textureColor = aTexture;\n"
		"	Normal = aNormal;\n"
		"	fragColor = aPos * aColor;\n"
		"}\0";

	const char *fragmentShaderSource = "#version 330 core\n"
		"in vec3 fragColor;\n"
		"\n"
		"out vec4 outColor;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	outColor = vec4(fragColor, 1.0);\n"
		"}\0";

public:
	Shader();
	~Shader();

	void useProgram();
	void setUniformMatrix4x4(const float *matrix, const char *name);
	void setUniformMatrix3x3(const float *matrix, const char *name);
	void setUniform1i(const char* name, int value);
	void setUniform3f(const char* name, float* value);

	void changeMode();
	int getMode();
};

#endif

