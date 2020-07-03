#ifndef LIST_GRAPH_HEADER_H_INCLUDED
#define LIST_GRAPH_HEADER_H_INCLUDED
#include "Header.h"

using namespace std;

class ListGraph: public IGraph{
public:
    ListGraph( IGraph* );
    ListGraph(int n):graph(n){}

    void AddEdge( int, int );
    int VerticesCount() const;
    void GetNextVertices( int, vector<int>& ) const;
    void GetPrevVertices( int, vector<int>& ) const;
private:
    vector<vector<int>> graph;
};

#endif // LIST_GRAPH_HEADER_H_INCLUDED
