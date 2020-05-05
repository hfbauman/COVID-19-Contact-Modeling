#include "Circle.h"
#include <cmath>

Circle::Circle(vector<float> position, float radius, int vertex_data)
{
	Circle::position = position;
	Circle::radius = radius;
	Circle::vertex_data = vertex_data;
	
	//Initialize the color to be white
	Circle::color.push_back(1.0);
	Circle::color.push_back(1.0);
	Circle::color.push_back(1.0);
}

Circle::Circle(float x, float y, float radius, int vertex_data)
{
	Circle::x = x;
	Circle::y = y;
	Circle::position.push_back(x);
	Circle::position.push_back(y);
	Circle::radius = radius;
	Circle::vertex_data = vertex_data;

	//Initialize the color to be white
	Circle::color.push_back(1.0);
	Circle::color.push_back(1.0);
	Circle::color.push_back(1.0);
}

void Circle::setPosition(float x, float y)
{
	Circle::x = x;
	Circle::y = y;
	Circle::position[0] = x;
	Circle::position[1] = y;
}

void Circle::setColor(vector<float> color)
{
	Circle::color = color;
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

vector<float> Circle::getPosition()
{
	return position;
}

vector<float> Circle::getColor()
{
	return color;
}

