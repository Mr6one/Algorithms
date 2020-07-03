#include "Header.h"
#include "Arc_Graph_header.h"
#include "List_Graph_header.h"
#include "Matrix_Graph_header.h"
#include "Set_Graph_header.h"

#include <iostream>

using namespace std;

//пример работоспособности. Граф ориентированный.
//по входным данным строится ListGraph, на основе которого строится MatrixGraph. У MatrixGraph вызывется метод получения для каждой вершины вершины, смежной с данной.
int main()
{
    int n, m;
    cin>>n>>m;
    ListGraph graph(n);
    for (int i=0; i<m; ++i){
        int from, to;
        cin>>from>>to;
        graph.AddEdge(from, to);

    }
    MatrixGraph matrixGraph(&graph);
//assert
    for (int i=0; i<n; ++i){
        vector<int> vertices;
        matrixGraph.GetNextVertices(i, vertices);
        cout<<endl;
        cout<<i<<'-';
        for (int vert: vertices){
            cout<<vert<<' ';
        }
    }

    return 0;
}
