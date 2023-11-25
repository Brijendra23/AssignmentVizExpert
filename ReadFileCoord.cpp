#include "ReadFileCoord.h"

ReadFileCoord::ReadFileCoord()
{
}

std::vector<Point> ReadFileCoord::getPointsFromFile(std::string filePath)
{
	std::vector<Point> points;
	std::ifstream file(filePath);
	std::string line;
	while (std::getline(file, line))
	{
		float x, y;
		std::istringstream iss(line);
		iss >> x >> y;
		points.push_back(Point{ x,y });
	}
	if (points.size() == NULL)
	{
		printf("Error reading points");
	}
	return points;
}

ReadFileCoord::~ReadFileCoord()
{
}
