#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Triangulation
{
public:
	struct Point { pair<float, float> x; float angle = 0.0; };
	struct triangulation { int x; int y; int z; };
	Triangulation();
	~Triangulation();
	static float angle_in_point(Point prev, Point curr, Point next);
	static vector<Point> calc_angles(vector<Point> data);
	static void step(vector<Triangulation::triangulation>& triangulation, vector<Point>& points, vector<int>& border);
	static bool is_inner_point(Point a, Point b, Point c, Point x);

private:


};
