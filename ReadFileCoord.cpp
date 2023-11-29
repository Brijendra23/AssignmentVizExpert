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
        char c;//for covering comma in text file
		std::istringstream iss(line);
		iss >> x >> c >> y;
		points.push_back(Point{ (x),y });
	}
	if (points.size() == NULL)
	{
		printf("Error reading points");
	}
	return points;


}
std::vector<GLfloat> ReadFileCoord::calculateSplineCoords(Point p, Point p1, Point p2, Point p3, float widthGeometery)
{
	std::vector<GLfloat> points;
	for (float i = 0; i < 1; i += 0.01)
	{
		//calculation of vertices for extruding a geometry along Catmull-ROM
		float x = 0.5 * ((-(i * i * i) + 2 * (i * i) - i) * p.x + (3 * (i * i * i) - 5 * (i * i) + 2) * p1.x + (-3 * (i * i * i) + 4 * (i * i) + i) * p2.x + ((i * i * i) - (i * i)) * p3.x);
	    float y = 0.5 * ((-(i * i * i) + 2 * (i * i) - i) * p.y + (3 * (i * i * i) - 5 * (i * i) + 2) * p1.y + (-3 * (i * i * i) + 4 * (i * i) + i) * p2.y + ((i * i * i) - (i * i)) * p3.y);
		
		
	
			//cases for generating the coordinates for u-v mapping of the texture
			float angle = atan2(p2.y - y, p2.x - x);
			points.push_back(x + cos(angle) * widthGeometery);
			points.push_back(y + sin(angle) * widthGeometery);

			points.push_back(x - cos(angle) * widthGeometery);
			points.push_back(y - sin(angle) * widthGeometery);
	}

	return points;
}

ReadFileCoord::~ReadFileCoord()
{
}
