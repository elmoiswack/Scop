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
	std::vector<unsigned char> textureData;
	int textureWidth = 0;
	int textureHeight = 0;

	Vertex centerVertex;
	
public:
	FileParser(const std::string& pathToFile);
	FileParser(const std::string& pathToFile, const std::string& pathToTexture);
	~FileParser();

	void parseInputFile(const std::string& pathToFile);
	void parseInputTexture(const std::string& pathToTexture);

	void parseFaceLine(const std::string& line);
	std::vector<std::string> SplitByDelim(const std::string& line, char delim);
	int amountDelimInLine(const std::string& line, char delim);

	void computeCenterVertex();
	Vertex getCenterVertex();
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