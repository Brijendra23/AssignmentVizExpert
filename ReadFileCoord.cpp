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
        char c;
		std::istringstream iss(line);
		iss >> x >> c >> y;
		points.push_back(Point{ x,y });
	}
	if (points.size() == NULL)
	{
		printf("Error reading points");
	}
	return points;



   /* std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return ;
    }

    std::vector<Point> points;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string token;
        Point p;

        // Split the line by commas
        while (std::getline(ss, token, ',')) {
            if (token.empty()) {
                std::cerr << "Invalid data format!" << std::endl;
               
            }

            if (!(std::istringstream(token) >> p.x)) {
                std::cerr << "Invalid x-coordinate!" << std::endl;
               
            }

            if (std::getline(ss, token, ',')) {
                if (!(std::istringstream(token) >> p.y)) {
                    std::cerr << "Invalid y-coordinate!" << std::endl;
           
                }
            }
            else {
                std::cerr << "Missing y-coordinate!" << std::endl;
             
            }
        }

        points.push_back(p);
    }
    inputFile.close();*/
    

}

ReadFileCoord::~ReadFileCoord()
{
}
