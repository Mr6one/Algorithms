#ifndef MATRIX_GRAPH_HEADER_H_INCLUDED
#define MATRIX_GRAPH_HEADER_H_INCLUDED
#include "Header.h"

using namespace std;

class MatrixGraph: public  IGraph{
public:
    MatrixGraph(IGraph*);
    MatrixGraph(int n):graph(n){}

    void AddEdge(int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<int>&)const;
    void GetPrevVertices(int, vector<int>&)const;
private:
    vector<vector<int>> graph;
};

#endif // MATRIX_GRAPH_HEADER_H_INCLUDED
