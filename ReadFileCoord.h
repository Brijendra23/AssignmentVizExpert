#pragma once
#include<iostream>
#include <vector>
#include<fstream>
#include <string>
#include<sstream>
using namespace std;
#include"Utils.h"




class ReadFileCoord
{
public:
	ReadFileCoord();
	std::vector<Point> getPointsFromFile(std::string filePath);
	~ReadFileCoord();
};

