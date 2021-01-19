#pragma once

#include <vector>
#include <fstream>
#include <queue>
#include "../SAVM/Matrix.h"

using namespace std;

class CathillMckeeAlgorithm
{
private:
	Matrix<int>_matrix;
	int numNodes;
public:
	Matrix<int> ReverseCuthillMckee(Matrix<double>& coordinates, Matrix<double>& restrictions);
	vector<vector<int>> degreeGenerator();
	void setNeighbors(vector<int>&, int, int);
	vector<int> CuthillMckee();
	CathillMckeeAlgorithm(Matrix<int> m, int);
	~CathillMckeeAlgorithm() {}
};

