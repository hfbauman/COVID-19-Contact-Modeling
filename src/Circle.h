#pragma once
#include <vector>
using namespace std;
class Circle
{
	vector<double> position;
	vector<float> color;
	vector<double> velocity;
	double x;
	double y;
	double radius;
	int vertex_data;

public:
	Circle(vector<double> position, double radius, int vertex_data);
	Circle(double x=0.0, double y=0.0, double radius=1.0, int vertex_data=0);
	void setPosition(vector<double> position);
	void setColor(vector<float> color);
	void setVelocity(vector<double> velocity);
	int getVertexData();
	double getRadius();
	double getX();
	double getY();
	vector<double> getPosition();
	vector<float> getColor();
	vector<double> getVelocity();
};

