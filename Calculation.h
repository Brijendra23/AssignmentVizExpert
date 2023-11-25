#pragma once
#include<iostream>
#include<gl/glew.h>
#include <vector>
#include"Utils.h"
class Calculation
{
public:
	Calculation();
	std::vector<GLfloat> calculateSplineCoords(Point p, Point p1, Point p2, Point p3, float widthGeometery);
	~Calculation();
};

