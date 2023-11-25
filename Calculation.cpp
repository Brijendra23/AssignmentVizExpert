#include "Calculation.h"

Calculation::Calculation()
{
}

std::vector<GLfloat> Calculation::calculateSplineCoords(Point p, Point p1, Point p2, Point p3, float widthGeometery)
{
	std::vector<GLfloat> points;
	for (float i = 0; i < 1; i += 0.01)
	{
		float x = 0.5 * ((2 * p1.x) + (-p.x + p2.x) * i + (2 * p.x - 5 * p1.x + 4 * p2.x - p3.x) * i * i + (-p.x + 3 * p1.x - 3 * p2.x + p3.x) * i * i * i);
		float y = 0.5 * ((2 * p1.y) + (-p.y + p2.y) * i + (2 * p.y - 5 * p1.y + 4 * p2.y - p3.y) * i * i + (-p.y + 3 * p1.y - 3 * p2.y + p3.y) * i * i * i);
		float angle = atan2(p2.y - y, p2.x - x);
		points.push_back(x + cos(angle) * widthGeometery);
		points.push_back(y + sin(angle) * widthGeometery);
		points.push_back(x - cos(angle) * widthGeometery);
		points.push_back(y - sin(angle) * widthGeometery);
	}
	return points;
}

Calculation::~Calculation()
{
}
