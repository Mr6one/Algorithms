#ifndef SET_GRAPH_HEADER_H_INCLUDED
#define SET_GRAPH_HEADER_H_INCLUDED
#include "Header.h"
#include <unordered_set>

using namespace std;

class SetGraph: public  IGraph{
public:
    SetGraph(IGraph*);
    SetGraph(int n): graph(n){}

    void AddEdge(int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<int>&)const;
    void GetPrevVertices(int, vector<int>&)const;
private:
    vector<unordered_set<int>> graph;
};


#endif // SET_GRAPH_HEADER_H_INCLUDED
