#ifndef FILEPARSER_HPP
# define FILEPARSER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

class FileParser
{
private:
	std::vector<float> vertices;
	std::vector<int> fragments;
public:
	FileParser(char *pathToFile);
	~FileParser();

	std::vector<float> getVertices();
	std::vector<int> getFragments();
};

#endif