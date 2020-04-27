#pragma once
class Circle
{
	float *vertices;
	float x;
	float y;
	float z;
	float radius;

public:
	Circle(float x, float y, float z, float radius, int resolution);
	~Circle();
	void setPosition(float x, float y, float z);
	void setRadius(float radius);
};

