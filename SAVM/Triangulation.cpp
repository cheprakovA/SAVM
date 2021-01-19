#include "Triangulation.h"

Triangulation::Triangulation()
{
}

Triangulation::~Triangulation()
{
}

float Triangulation::angle_in_point(Point prev, Point curr, Point nxt)
{
	float a_0, b_0, a_1, b_1, vmul, aabs, babs, cosa;
	a_0 = prev.x.first - curr.x.first;
	a_1 = prev.x.second - curr.x.second;
	b_0 = nxt.x.first - curr.x.first;
	b_1 = nxt.x.second - curr.x.second;
	vmul = a_0 * b_0 + a_1 * b_1;
	aabs = sqrt(pow(a_0, 2) + pow(a_1, 2));
	babs = sqrt(pow(b_0, 2) + pow(b_1, 2));
	cosa = vmul / (aabs * babs);
	return floor(acos(cosa) * 180.0 / M_PI * 100 + 0.5) * 0.01;
}

vector<Triangulation::Point> Triangulation::calc_angles(vector<Point> data)
{
	int n = data.size();
	vector<Point> points;
	Point p;
	p.angle = angle_in_point(data[n - 1], data[0], data[1]);
	p.x = data[0].x;
	points.push_back(p);

	for (int i = 0; i < n - 2; i++) {
		p.angle = angle_in_point(data[i], data[i + 1], data[i + 2]);
		p.x = data[i + 1].x;
		points.push_back(p);
	}
	p.angle = angle_in_point(data[n - 2], data[n - 1], data[0]);
	p.x = data[n - 1].x;
	points.push_back(p);
	return points;
}

bool Triangulation::is_inner_point(Point a, Point b, Point c, Point x)
{
	float first = (a.x.first - x.x.first) * (b.x.second - a.x.second) - (b.x.first - a.x.first) * (a.x.second - x.x.second);
	float second = (b.x.first - x.x.first) * (c.x.second - b.x.second) - (c.x.first - b.x.first) * (b.x.second - x.x.second);
	float third = (c.x.first - x.x.first) * (a.x.second - c.x.second) - (a.x.first - c.x.first) * (c.x.second - x.x.second);
	return ((first < 0 and second < 0 and third < 0) or (first > 0 and second > 0 and third > 0));
}

void Triangulation::step(vector<Triangulation::triangulation>& triangulation, vector<Point>& points, vector<int>& border)
{
	vector<float> angles;
	for (auto i : border) {
		angles.push_back(points[i].angle);
	}
	auto ima = distance(angles.begin(), min_element(angles.begin(), angles.end()));
	Point a;
	Point b;
	Point c;
	int last_index;
	if (ima == 0) {
		a = points[border[0]];
		b = points[border[border.size() - 1]];
		last_index = 1;
		c = points[border[last_index]];
	}
	else {
		a = points[border[ima]];
		b = points[border[ima - 1]];

		if (ima == border.size() - 1) {
			last_index = 0;
		}
		else {
			last_index = ima + 1;
		}
		c = points[last_index];

	}
	Point a_; a_.x = make_pair(0, 0); a_.angle = a.angle;
	Point b_; b_.x = make_pair(b.x.first - a.x.first, b.x.second - a.x.second); b_.angle = b.angle;
	Point c_; c_.x = make_pair(c.x.first - a.x.first, c.x.second - a.x.second); c_.angle = c.angle;

	float lab = sqrt(pow(b_.x.first, 2) + pow(b_.x.second, 2));
	float lac = sqrt(pow(c_.x.first, 2) + pow(c_.x.second, 2));
	float lbc = sqrt(pow(c_.x.first - b_.x.first, 2) + pow(c_.x.second - b_.x.second, 2));

	if (a.angle >= 90.0) {
		float lap = (lab + lac) / 4;
		float  aap = acos(abs(c_.x.first) / lac) * 180.0 / M_PI + a_.angle / 2;

		int cos_sign;
		if (c_.x.first < 0) {
			cos_sign = -1;
		}
		else {
			cos_sign = 1;
		}

		int sin_sign;
		if (c_.x.second < 0) {
			sin_sign = -1;
		}
		else {
			sin_sign = 1;
		}

		float dx = cos(aap * M_PI / 180.0) * cos_sign * lap;
		float dy = sin(aap * M_PI / 180.0) * sin_sign * lap;

		float ll = 2.0 * lab * lac * cos(a.angle * M_PI / (2.0 * 180.0)) / (lab + lac);

		float lcp = sqrt(pow(c_.x.first, 2) + pow(c_.x.second, 2));
		float lbp = sqrt(pow(b_.x.first, 2) + pow(b_.x.second, 2));

		b_.angle = b.angle - asin(lap * sin(a_.angle * M_PI / (2.0 * 180.0)) / lbp) * 180.0 / M_PI;
		c_.angle = c.angle - asin(lap * sin(a_.angle * M_PI / (2.0 * 180.0)) / lcp) * 180.0 / M_PI;

		Point p_; p_.x = make_pair(a_.x.first + dx, a_.x.second + dy); p_.angle = 0;

		Point a__; a__.x = make_pair(a_.x.first - p_.x.first, a_.x.second - p_.x.second); a__.angle = a_.angle;
		Point b__; b__.x = make_pair(b_.x.first - p_.x.first, b_.x.second - p_.x.second); b__.angle = b_.angle;
		Point c__; c__.x = make_pair(c_.x.first - p_.x.first, c_.x.second - p_.x.second); c__.angle = c_.angle;
		Point p__; p__.x = make_pair(0, 0); p__.angle = p_.angle;

		float an_ = angle_in_point(b__, p__, c__);

		if (is_inner_point(a__, b__, c__, p__)) {
			p_.angle = an_;
		}
		else {
			p_.angle = 360 - an_;
		}

		Point p; p.x = make_pair(p_.x.first + a.x.first, p_.x.second + a.x.second); p.angle = p_.angle;

		int ip = points.size();
		points.push_back(p);

		triangulation.push_back({ border[ima], ip, border[last_index] });
		triangulation.push_back({ border[ima], ip, border[ima - 1] });

		auto it = border.begin();
		border[ima] = ip;

		points[border[ima - 1]].angle = b_.angle;
		points[border[last_index]].angle = c_.angle;

	}

	else {
		if (ima == 0) {
			points[border[border.size() - 1]].angle = b_.angle - asin(sin(a_.angle * M_PI / 180.0) * lac / lbc) * 180.0 / M_PI;
			points[border[last_index]].angle = c_.angle - asin(sin(a_.angle * M_PI / 180.0) * lab / lbc) * 180.0 / M_PI;
			triangulation.push_back({ border[border.size() - 1], border[ima], border[last_index] });
		}
		else {
			points[border[ima - 1]].angle = b_.angle - asin(sin(a_.angle * M_PI / 180.0) * lac / lbc) * 180.0 / M_PI;
			points[border[last_index]].angle = c_.angle - asin(sin(a_.angle * M_PI / 180.0) * lab / lbc) * 180.0 / M_PI;
			triangulation.push_back({ border[ima - 1], border[ima], border[last_index] });
		}
		auto it = border.begin();
		border.erase(it + ima);
	}

}