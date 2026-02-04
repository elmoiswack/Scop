#ifndef FILEPARSER_HPP
# define FILEPARSER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <string>

struct Vertex {
	float x;
	float y;
	float z;
};

struct TextureVertice {
	float x;
	float y;
};

struct Face {
	int vertice;
	int texture;
	int normal;
};

struct ComputedVertex {
	Vertex pos;
	TextureVertice texture;
};

class FileParser
{
private:
	std::vector<Vertex> vertices = {};
	std::vector<TextureVertice> textureVertices = {};
	std::vector<std::vector<Face>> faces = {};
	std::vector<ComputedVertex> computedVertex = {};
	bool textureAvailable = false;
	unsigned char* textureData;
	int textureWidth = 0;
	int textureHeight = 0;
	
public:
	FileParser();
	FileParser(std::string pathToFile);
	FileParser(std::string pathToFile, std::string pathToTexture);
	~FileParser();

	void parseInputFile(std::string pathToFile);
	void parseInputTexture(std::string pathToTexture);

	void parseFaceLine(std::string line);
	std::vector<std::string> SplitByDelim(std::string line, char delim);
	int amountDelimInLine(std::string line, char delim);

	void computeVertexes();
	TextureVertice getTextureFromFace(const Face& single);
	
	std::vector<Vertex> getVertices();
	std::vector<std::vector<Face>> getFaces();
	std::vector<ComputedVertex> getComputedVertex();
	bool getTextureAvailable();
	unsigned char* getTextureData();
	int getTextureWidth();
	int getTextureHeight();
};

#endif