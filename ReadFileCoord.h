#pragma once
#include<iostream>
#include <vector>
#include<fstream>
#include <string>
#include<sstream>
#include<glfw/glfw3.h>
using namespace std;
#include"Utils.h"




class ReadFileCoord
{
public:
	ReadFileCoord();
	std::vector<Point> getPointsFromFile(std::string filePath);
	std::vector<GLfloat> calculateSplineCoords(Point p, Point p1, Point p2, Point p3, float widthGeometery);
	~ReadFileCoord();
};

