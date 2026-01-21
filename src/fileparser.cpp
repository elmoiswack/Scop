#include "../includes/fileparser.hpp"

FileParser::FileParser(char *pathToFile)
{
	std::ifstream objectFile(pathToFile);

	if (!objectFile) {
		exit(1);
	}

	std::string tmpFileLine, singleVertice, singleFragment;
	std::string v, x, y, z;

	while (!objectFile.eof())
	{
		getline(objectFile, tmpFileLine);
		std::stringstream ss(tmpFileLine);

		if (tmpFileLine[0] == 'v' && tmpFileLine[1] == ' ')
		{
			ss >> v >> x >> y >> z;
			Vertex single = {std::stof(x.c_str()), std::stof(y.c_str()), std::stof(z.c_str())};
			this->vertices.push_back(single);
		}
		else if (tmpFileLine[0] == 'v' && tmpFileLine[1] == 'n')
		{
			ss >> v >> x >> y >> z;
			Vertex single = {std::stof(x.c_str()), std::stof(y.c_str()), std::stof(z.c_str())};
			this->normalVertices.push_back(single);
		}
		else if (tmpFileLine[0] == 'v' && tmpFileLine[1] == 't')
		{
			ss >> v >> x >> y;
			TextureVertice single = {std::stof(x.c_str()), std::stof(y.c_str())};
			this->textureVertices.push_back(single);
		}
		else if (tmpFileLine[0] == 'f' && tmpFileLine[1] == ' ') {
			this->parseFaceLine(tmpFileLine);
		}
	}
	objectFile.close();
}

FileParser::~FileParser()
{
	this->vertices.clear();
	this->faces.clear();
}

void FileParser::parseFaceLine(std::string line) { 
	
	line.erase(0, 2);
	auto faceArray = this->SplitByDelim(line,  ' ');
	std::vector<Face> facesFromLine = {};

	for (std::size_t i = 0; i < faceArray.size(); i++)
	{
		if (std::find(line.begin(), line.end(), '/') != line.end()) {
			auto tmp = this->SplitByDelim(faceArray[i], '/');
			
			if (this->amountDelimInLine(line, '/') == 1) {
				Face single = {(std::stoi(tmp[0]) - 1), (std::stoi(tmp[1]) - 1), -1};
				facesFromLine.push_back(single);
			
			} else {
				auto pos = std::find(line.begin(), line.end(), '/');
				if (pos != line.end())
					pos++;

				if (*pos == '/') {
					Face single = {(std::stoi(tmp[0]) - 1), -1, (std::stoi(tmp[1]) - 1)};
					facesFromLine.push_back(single);
				} else {
					Face single = {std::stoi(tmp[0]) - 1, (std::stoi(tmp[1]) - 1),  (std::stoi(tmp[2]) - 1)};
					facesFromLine.push_back(single);
				}
			}
		} else {
			Face single = {(std::stoi(faceArray[i]) - 1), -1, -1};
			facesFromLine.push_back(single);
		}
	}

	this->faces.push_back(facesFromLine);
}

int FileParser::amountDelimInLine(std::string line, char delim) {
	
	int count = 0;

	for (auto it = line.begin(); it != line.end(); it++) {
		if ((*it) == delim)
			count += 1;
	}

	return (count);
}

std::vector<std::string> FileParser::SplitByDelim(std::string line, char delim) {

	std::vector<std::string> array = {};
	std::size_t start = 0, index = 0;

	while (index < line.size())
	{
		if (line[index] == delim) {
			try {
				array.push_back(line.substr(start, index - start));
			} catch (std::out_of_range) {
				std::cout << "bruh" << std::endl;
				exit(1);
			}
			
			while (line[index] && line[index] == delim)
				index++;

			start = index;
		}
		index++;
	}

	array.push_back(line.substr(start, line.size() - start));
	return (array);
}

TextureVertice FileParser::getTextureFromFace(const Face& single) {
	if ((single.texture) < 0)
		return {0.0f, 0.0f};

	return (this->textureVertices[single.texture]);
}

Vertex FileParser::getNormalFromFace(const Face& single) {
	if ((single.normal) < 0)
		return {0.0f, 0.0f, 0.0f};

	return (this->normalVertices[single.normal]);
}

void FileParser::computeVertexes() {
	this->computedVertex = {};

	for (std::size_t index = 0; index < this->faces.size(); index++) {
		std::size_t first = 0, second = 1, third = 2;

		while (third < this->faces[index].size()) {
			auto firstFace = this->faces[index][first];
			auto secondFace = this->faces[index][second];
			auto thirdFace = this->faces[index][third];

			this->computedVertex.push_back({this->vertices[firstFace.vertice], getTextureFromFace(firstFace), getNormalFromFace(firstFace)});
			this->computedVertex.push_back({this->vertices[secondFace.vertice], getTextureFromFace(secondFace), getNormalFromFace(secondFace)});
			this->computedVertex.push_back({this->vertices[thirdFace.vertice], getTextureFromFace(thirdFace), getNormalFromFace(thirdFace)});

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