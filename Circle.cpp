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

	//Initialize velocity to be 0
	Circle::velocity.push_back(-0.01);
	Circle::velocity.push_back(0.0);
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

	//Initialize velocity to be 0
	Circle::velocity.push_back(0.0);
	Circle::velocity.push_back(0.0);
}

void Circle::setPosition(vector<float> position)
{
	Circle::position = position;
	x = Circle::position[0];
	y = Circle::position[1];
}

void Circle::setColor(vector<float> color)
{
	Circle::color = color;
}

void Circle::setVelocity(vector<float> velocity)
{
	Circle::velocity = velocity;
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

vector<float> Circle::getVelocity()
{
	return velocity;
}

