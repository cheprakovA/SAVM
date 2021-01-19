#include "Algorithm.h"

void Algorithm::process()
{

	ofstream myfile;
	myfile.open("matrix.txt");

	ofstream coords;
	coords.open("coordinates.txt");


	Triangulation::Point p;
	vector<Triangulation::Point> data;

	//{ { 6, 3 }, { 3, 6 }, { 3, 9 }, { 6, 12 }, { 11, 14 }, { 15, 8 }, { 12, 4 } }

	p.x = make_pair(6, 3);
	p.angle = 0.0;
	data.push_back(p);
	p.x = make_pair(3, 6);
	data.push_back(p);
	p.x = make_pair(3, 9);
	data.push_back(p);
	p.x = make_pair(6, 12);
	data.push_back(p);
	p.x = make_pair(11, 14);
	data.push_back(p);
	p.x = make_pair(15, 8);
	data.push_back(p);
	p.x = make_pair(12, 4);
	data.push_back(p);

	vector<Triangulation::Point> points = Triangulation::calc_angles(data);

	vector<int> border;
	for (int i = 0; i < points.size(); i++) {
		border.push_back(i);
	}

	vector<Triangulation::triangulation> triangulation;


	while (border.size() > 3) {
		Triangulation::step(triangulation, points, border);
	}
	triangulation.push_back({ border[0],border[1],border[2] });

	for (auto i : triangulation) {
		myfile << i.x << " " << i.y << " " << i.z << endl;
	}

	myfile.close();

	int num_rows = 7, num_columns = 3, num_nodes = 8;

	Matrix<int> matrix = Matrix<int>(num_rows, num_columns);
	ifstream file_with_matrix("matrix.txt");
	file_with_matrix >> matrix;
	file_with_matrix.close();

	for (auto i : points) {
		coords << i.x.first << " " << i.x.second << endl;
	}

	coords.close();

	Matrix<double> coordinates = Matrix<double>(num_nodes, 2);
	ifstream file_with_coordinates("coordinates.txt");
	file_with_coordinates >> coordinates;

	file_with_coordinates.close();



}