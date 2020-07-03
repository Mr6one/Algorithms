#include "Header.h"
#include "Matrix_Graph_header.h"

MatrixGraph::MatrixGraph(IGraph* InitialGraph){
    int size_=InitialGraph->VerticesCount();
    graph.resize(size_);
    for (int i=0; i<size_; ++i){
        graph[i].resize(size_, 0);
    }
    vector<int> vertices(size_);
    for (int i=0; i<size_; ++i){
        InitialGraph->GetNextVertices(i, vertices);
        for (vert: vertices){
            graph[i][vert]=1;
        }
        vertices.clear();
    }
}

void MatrixGraph::AddEdge(int from, int to){
    graph[from][to]=1;
}

int MatrixGraph::VerticesCount()const{
    return graph.size();
}

void MatrixGraph::GetNextVertices(int vertex, vector<int>& vertices)const{
    for (size_t i=0; i<graph.size(); ++i){
        if (graph[vertex][i]==1){
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int vertex, vector<int>& vertices)const{
    for (size_t i=0; i<graph.size(); ++i){
        if (graph[i][vertex]==1){
            vertices.push_back(i);
        }
    }
}
