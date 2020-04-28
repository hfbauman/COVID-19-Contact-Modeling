#pragma once
#include <vector>
using namespace std;
class Circle
{
	float x;
	float y;
	float radius;
	int vertex_data;

public:
	Circle(float x=0.0, float y=0.0, float radius=1.0, int vertex_data=0);
	void setPosition(float x, float y);
	int getVertexData();
	float getRadius();
	float getX();
	float getY();
};

