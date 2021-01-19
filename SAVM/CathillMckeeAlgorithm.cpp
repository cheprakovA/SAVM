#include "CathillMckeeAlgorithm.h"

using namespace std;

vector<vector<int>> globalDegree;

int findIndex(vector<pair<int, int> > a, int x)
{
    for (int i = 0; i < a.size(); i++)
        if (a[i].first == x)
            return i;
    return -1;
}

bool compareDegree(int i, int j)
{
    return ::globalDegree[i] < ::globalDegree[j];
}

CathillMckeeAlgorithm::CathillMckeeAlgorithm(Matrix<int> m, int countOfNodes)
{
    _matrix = m;
    numNodes = countOfNodes;
}

void CathillMckeeAlgorithm::setNeighbors(vector<int>& exist_neighbors, int row, int col)
{
    for (int i = 0; i < _matrix.getCols(); i++)
    {
        if (i != col && find(exist_neighbors.begin(), exist_neighbors.end(), _matrix(row, i)) == exist_neighbors.end())
        {
            exist_neighbors.push_back(_matrix(row, i));
        }
    }
}

vector<vector<int>> CathillMckeeAlgorithm::degreeGenerator()
{
    vector<vector<int>> degrees;
    vector<vector<int>> v;
    for (int i = 0; i < numNodes; i++)
    {
        v.push_back({});
    }
    for (int i = 0; i < _matrix.getRows(); i++)
    {
        for (int j = 0; j < _matrix.getCols(); j++)
        {
            int current_node = _matrix(i, j);
            setNeighbors(v[current_node], i, j);
        }
    }
    for (int i = 0; i < v.size(); i++)
    {
        degrees.push_back(v[i]);
    }
    return degrees;
}

bool isExist(vector<int> nodes, int i) {
    return find(nodes.begin(), nodes.end(), i) != nodes.end();
}

vector<int> CathillMckeeAlgorithm::CuthillMckee()
{
    vector<vector<int>> degrees = degreeGenerator();

    ::globalDegree = degrees;

    queue<int> Q;
    vector<int> R;
    vector<pair<int, int> > notVisited;

    for (int i = 0; i < degrees.size(); i++)
        notVisited.push_back(make_pair(i, degrees[i].size()));

    while (notVisited.size())
    {

        int minNodeIndex = 0;

        for (int i = 0; i < notVisited.size(); i++)
            if (notVisited[i].second < notVisited[minNodeIndex].second)
                minNodeIndex = i;

        Q.push(notVisited[minNodeIndex].first);

        notVisited.erase(notVisited.begin() + findIndex(notVisited, notVisited[Q.front()].first));
        while (!Q.empty())
        {
            vector<int> toSort;

            for (int i = 0; i < numNodes; i++)
            {
                if (i != Q.front() && isExist(degrees[Q.front()], i) && findIndex(notVisited, i) != -1)
                {
                    toSort.push_back(i);
                    notVisited.erase(notVisited.begin() + findIndex(notVisited, i));
                }
            }

            sort(toSort.begin(), toSort.end(), compareDegree);

            for (int i = 0; i < toSort.size(); i++)
                Q.push(toSort[i]);

            R.push_back(Q.front());
            Q.pop();
        }
    }

    return R;
}

Matrix<int> CathillMckeeAlgorithm::ReverseCuthillMckee(Matrix<double>& coordinates, Matrix<double>& restrictions)
{
    vector<int> cuthill = CuthillMckee();

    int n = cuthill.size();

    if (n % 2 == 0)
        n -= 1;

    n = n / 2;

    for (int i = 0; i <= n; i++)
    {
        int j = cuthill[cuthill.size() - 1 - i];
        cuthill[cuthill.size() - 1 - i] = cuthill[i];
        cuthill[i] = j;
    }

    Matrix<int> buffMatrix = _matrix;
    int count = 0;
    while (count < numNodes)
    {
        for (int i = 0; i < _matrix.getRows(); i++)
        {
            for (int j = 0; j < _matrix.getCols(); j++)
            {
                if (buffMatrix(i, j) == count)
                {
                    _matrix(i, j) = cuthill[count];
                }
            }
        }
        count++;
    }

    Matrix<double> buffCoordinates = coordinates, buffRestrictions = restrictions;
    for (int i = 0; i < coordinates.getRows(); i++)
    {
        for (int j = 0; j < coordinates.getCols(); j++)
        {
            coordinates(i, j) = buffCoordinates(cuthill[i], j);
        }
    }
    for (int i = 0; i < restrictions.getRows(); i++)
    {
        int numNodesOfRestriction = restrictions(i, 0);
        restrictions(i, 0) = cuthill[numNodesOfRestriction];
    }

    return _matrix;
}