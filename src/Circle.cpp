#include "Circle.h"
#include <cmath>

Circle::Circle(vector<double> position, double radius, int vertex_data)
{
	Circle::position = position;
	Circle::radius = radius;
	Circle::vertex_data = vertex_data;

	infection_count = 0;
	
	//Initialize the color to be white
	Circle::color.push_back(1.0);
	Circle::color.push_back(1.0);
	Circle::color.push_back(1.0);

	//Initialize velocity to be 0
	Circle::velocity.push_back(0.0);
	Circle::velocity.push_back(0.0);
}

Circle::Circle(double x, double y, double radius, int vertex_data)
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

	//Initialize velocity to be 0
	Circle::velocity.push_back(0.0);
	Circle::velocity.push_back(0.0);
}

void Circle::setPosition(vector<double> position)
{
	Circle::position = position;
	x = Circle::position[0];
	y = Circle::position[1];
}

void Circle::setColor(vector<float> color)
{
	Circle::color = color;
}

void Circle::setVelocity(vector<double> velocity)
{
	Circle::velocity = velocity;
}

int Circle::getVertexData()
{
	return vertex_data;
}

double Circle::getRadius()
{
	return radius;
}

double Circle::getX()
{
	return x;
}

double Circle::getY()
{
	return y;
}

vector<double> Circle::getPosition()
{
	return position;
}

vector<float> Circle::getColor()
{
	return color;
}

vector<double> Circle::getVelocity()
{
	return velocity;
}

