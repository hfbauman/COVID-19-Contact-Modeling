#pragma once
#include <vector>
using namespace std;
class Circle
{
	vector<float> position;
	vector<float> color;
	vector<float> velocity;
	float x;
	float y;
	float radius;
	int vertex_data;

public:
	Circle(vector<float> position, float radius, int vertex_data);
	Circle(float x=0.0, float y=0.0, float radius=1.0, int vertex_data=0);
	void setPosition(vector<float> position);
	void setColor(vector<float> color);
	void setVelocity(vector<float> velocity);
	int getVertexData();
	float getRadius();
	float getX();
	float getY();
	vector<float> getPosition();
	vector<float> getColor();
	vector<float> getVelocity();
};

