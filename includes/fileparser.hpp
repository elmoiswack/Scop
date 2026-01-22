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
	Vertex normal;
};

struct MTLLIB {
	std::string name = "";
	float specularExponent = 0.0f;
	float opticalDensity = 0.0f;
	float dissolve = 0.0f;
	std::vector<float> ambientColor = {0.0f, 0.0f, 0.0f};
	std::vector<float> diffuseColor = {0.0f, 0.0f, 0.0f};
	std::vector<float> specularColor = {0.0f, 0.0f, 0.0f};
	int illumination = 0;
};

class FileParser
{
private:
	std::vector<Vertex> vertices = {};
	std::vector<Vertex> normalVertices = {};
	std::vector<TextureVertice> textureVertices = {};
	std::vector<std::vector<Face>> faces = {};
	std::vector<ComputedVertex> computedVertex = {};
	std::vector<MTLLIB> materials = {};
	std::vector<std::string> useMaterials = {};
	
public:
	FileParser(std::string pathToFile);
	~FileParser();

	void parseMtllibFile(std::string line);
	std::vector<float> parseRGB(std::vector<std::string>& tokens);

	void parseFaceLine(std::string line);
	std::vector<std::string> SplitByDelim(std::string line, char delim);
	int amountDelimInLine(std::string line, char delim);

	void computeVertexes();
	TextureVertice getTextureFromFace(const Face& single);
	Vertex getNormalFromFace(const Face& single);
	
	std::vector<Vertex> getVertices();
	std::vector<std::vector<Face>> getFaces();
	std::vector<ComputedVertex> getComputedVertex();
};

#endif