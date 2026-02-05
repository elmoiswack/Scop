#include "../includes/fileparser.hpp"

FileParser::FileParser(const std::string& pathToFile) {
	this->parseInputFile(pathToFile);
	this->textureAvailable = false;
}

FileParser::FileParser(const std::string& pathToFile, const std::string& pathToTexture) {
	this->parseInputFile(pathToFile);
	this->parseInputTexture(pathToTexture);
	this->textureAvailable = true;
}

FileParser::~FileParser() {
	this->vertices.clear();
	this->computedVertex.clear();
	this->textureVertices.clear();
	for (auto it = this->faces.begin(); it != this->faces.end(); it++)
		(*it).clear();
	this->faces.clear();
}

void FileParser::parseInputFile(const std::string& pathToFile) {
	std::ifstream objectFile(pathToFile);

	if (!objectFile) {
		exit(1);
	}

	std::string tmpFileLine;
	std::string v, x, y, z, name;

	while (std::getline(objectFile, tmpFileLine))
	{
		if (tmpFileLine.empty() || tmpFileLine[0] == '#') {
			continue;
		}

		std::stringstream ss(tmpFileLine);
		std::string prefix;
		ss >> prefix;

		if (prefix == "v") {
			float x, y, z;
			ss >> x >> y >> z;
			this->vertices.push_back({x, y, z});
		} 
		else if (prefix == "vt") {
			float x, y;
			ss >> x >> y;
			this->textureVertices.push_back({x, y});
		} 
		else if (prefix == "f") {
			this->parseFaceLine(tmpFileLine);
		}
	}
	objectFile.close();
}

void FileParser::parseFaceLine(const std::string& line) { 
    std::string faceLine = line.substr(2);
    auto faceArray = this->SplitByDelim(faceLine, ' ');
    std::vector<Face> facesFromLine;
    facesFromLine.reserve(faceArray.size());

    for (const auto& faceStr : faceArray)
    {
        if (faceStr.find('/') != std::string::npos) {
            auto tmp = this->SplitByDelim(faceStr, '/');
            
            if (tmp.size() == 2 && !tmp[1].empty()) {
                Face single = {std::stoi(tmp[0]) - 1, std::stoi(tmp[1]) - 1, -1};
                facesFromLine.push_back(single);
            }
            else if (tmp.size() == 3 && tmp[1].empty()) {
                Face single = {std::stoi(tmp[0]) - 1, -1, std::stoi(tmp[2]) - 1};
                facesFromLine.push_back(single);
            }
            else if (tmp.size() == 3 && !tmp[1].empty()) {
                Face single = {std::stoi(tmp[0]) - 1, std::stoi(tmp[1]) - 1, std::stoi(tmp[2]) - 1};
                facesFromLine.push_back(single);
            }
        } 
        else {
            Face single = {std::stoi(faceStr) - 1, -1, -1};
            facesFromLine.push_back(single);
        }
    }

    this->faces.push_back(std::move(facesFromLine));
}

int FileParser::amountDelimInLine(const std::string& line, char delim) {
	return std::count(line.begin(), line.end(), delim);
}

std::vector<std::string> FileParser::SplitByDelim(const std::string& line, char delim) {
    std::vector<std::string> array;
    std::size_t start = 0;

    for (std::size_t i = 0; i <= line.size(); i++)
    {
        if (i == line.size() || line[i] == delim) {
            array.push_back(line.substr(start, i - start));
            start = i + 1;
        }
    }

    return array;
}

void FileParser::parseInputTexture(const std::string& pathToTexture) {
    FILE* f = fopen(pathToTexture.c_str(), "rb");
    if (!f) {
        std::cout << "Failed to open texture file" << std::endl;
        exit(1);
    }
    
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); 

    this->textureWidth = *(int*)&info[18];
    this->textureHeight = *(int*)&info[22];

    int size = 3 * this->textureWidth * this->textureHeight;
    this->textureData.resize(size);

    fread(this->textureData.data(), sizeof(unsigned char), size, f);
    fclose(f);

    for (int i = 0; i < size; i += 3)
        std::swap(this->textureData[i], this->textureData[i+2]);
}

TextureVertice FileParser::getTextureFromFace(const Face& single) {
	if ((single.texture) < 0)
		return {0.0f, 0.0f};

	return (this->textureVertices[single.texture]);
}

void FileParser::computeVertexes() {
	this->computedVertex = {};

	for (std::size_t index = 0; index < this->faces.size(); index++) {
		std::size_t first = 0, second = 1, third = 2;

		while (third < this->faces[index].size()) {
			auto firstFace = this->faces[index][first];
			auto secondFace = this->faces[index][second];
			auto thirdFace = this->faces[index][third];

			this->computedVertex.push_back({this->vertices[firstFace.vertice], getTextureFromFace(firstFace)});
			this->computedVertex.push_back({this->vertices[secondFace.vertice], getTextureFromFace(secondFace)});
			this->computedVertex.push_back({this->vertices[thirdFace.vertice], getTextureFromFace(thirdFace)});

			second++;
			third++;
		}
	}
}

std::vector<Vertex> FileParser::getVertices() {
	return this->vertices;
}

std::vector<std::vector<Face>> FileParser::getFaces() {
	return this->faces;
}

std::vector<ComputedVertex> FileParser::getComputedVertex() {
	return this->computedVertex;
}

bool FileParser::getTextureAvailable() {
	return this->textureAvailable;
}

unsigned char *FileParser::getTextureData() {
	return this->textureData.data();
}

int FileParser::getTextureWidth() {
	return this->textureWidth;
}

int FileParser::getTextureHeight() {
	return this->textureHeight;
 }