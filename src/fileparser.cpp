#include "../includes/fileparser.hpp"

FileParser::FileParser(char *pathToFile)
{
	std::ifstream objectFile(pathToFile);

	if (!objectFile) {
		exit(1);
	}

	std::string tmpFileLine, singleVertice, singleFragment;
	std::string v, x, y, z;
	std::string f, r, g, b, a;

	while (!objectFile.eof())
	{
		getline(objectFile, tmpFileLine);
		std::stringstream ss(tmpFileLine);

		if (tmpFileLine[0] == 'v')
		{
			ss >> v >> x >> y >> z;
			
			this->vertices.push_back(std::stof(x));
			this->vertices.push_back(std::stof(y));
			this->vertices.push_back(std::stof(z));
		}
		else if (tmpFileLine[0] == 'f')
		{
			ss >> f >> r >> g >> b >> a;

			if (a.size() == 0) {
				a.push_back('0');
			}

			this->fragments.push_back(std::stoi(r));
			this->fragments.push_back(std::stoi(g));
			this->fragments.push_back(std::stoi(b));
			this->fragments.push_back(std::stoi(a));
		}
	}
	objectFile.close();
}

FileParser::~FileParser()
{
	this->vertices.clear();
	this->fragments.clear();
}

std::vector<float> FileParser::getVertices() {
	return this->vertices;
}

std::vector<int> FileParser::getFragments() {
	return this->fragments;
}