#ifndef ARCGRAPH_H_INCLUDED
#define ARCGRAPH_H_INCLUDED
#include "Header.h"

using namespace std;

class ArcGraph: public IGraph{
public:
    ArcGraph(IGraph*);
    ArcGraph(int n): graph(n){}

    void AddEdge(int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<int>&)const;
    void GetPrevVertices(int, vector<int>&)const;
private:
    vector<pair<int, int>> graph;
    int size_;
};

#endif // ARCGRAPH_H_INCLUDED
