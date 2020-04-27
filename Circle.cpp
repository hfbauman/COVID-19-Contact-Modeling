#include "Circle.h"
#include <cmath>

Circle::Circle(float x, float y, float z, float radius, int resolution)
{
	vertices = new float[(resolution + 2) * 3];
	vertices[0] = 0.0;
	vertices[0] = 0.0;
	vertices[0] = 0.0;

	//Sets the center to be(0, 0, 0) so that I can easily use the shader to move the object around
	vertices[0] = 0.0;
	vertices[1] = 0.0;
	vertices[2] = 0.0;

	//The angle from (0,1,0)
	float angle;

	//Generates vertices around the circle
	for (int i = 1; i < sizeof(vertices) / sizeof(vertices[0]) / 3 + 1;i++)
	{
		angle = (i - 1) * (2 * 3.14159) / resolution;
		vertices[3 * i] = sin(angle);
		vertices[3 * i + 1] = cos(angle);
		vertices[3 * i + 2] = 0.0;
	}

}

//Destructor function
Circle::~Circle()
{
	if (vertices) {
		delete[] vertices;
	}
}

//Allows us to set the position without giving access to the variables
void Circle::setPosition(float x, float y, float z)
{
	x = x;
	y = y;
	z = z;
}

//Allows us to set the radius without giving access to the radius
void Circle::setRadius(float radius)
{
	radius = radius;
}
