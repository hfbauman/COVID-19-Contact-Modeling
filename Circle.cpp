#include "Circle.h"
#include <cmath>

Circle::Circle(float x, float y, float radius, int vertex_data)
{
	Circle::x = x;
	Circle::y = y;
	Circle::radius = radius;
	Circle::vertex_data = vertex_data;
}

void Circle::setPosition(float x, float y)
{
	Circle::x = x;
	Circle::y = y;
}

int Circle::getVertexData()
{
	return vertex_data;
}

float Circle::getRadius()
{
	return radius;
}

float Circle::getX()
{
	return x;
}

float Circle::getY()
{
	return y;
}

