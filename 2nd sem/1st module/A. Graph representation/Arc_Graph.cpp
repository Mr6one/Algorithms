#include "Header.h"
#include "Arc_Graph_header.h"

ArcGraph::ArcGraph(IGraph* InitialGraph){
    size_=InitialGraph->VerticesCount();
    vector<int> vertices;
    for (int i=0; i<size_; ++i){
        InitialGraph->GetNextVertices(i, vertices);
        for (vert: vertices){
            graph.push_back(make_pair(i, vert));
        }
    }
}

void ArcGraph::AddEdge(int from, int to){
    graph.push_back(make_pair(from, to));
}

int ArcGraph::VerticesCount()const{
    return size_;
}

void ArcGraph::GetNextVertices(int vertex, vector<int>& vertices)const{
    for (int i=0; i<=size_; ++i){
        if (graph[i].first==vertex){
            vertices.push_back(graph[i].second);
        }
    }
}

void ArcGraph::GetPrevVertices(int vertex, vector<int>& vertices)const{
    for (int i=0; i<=size_; ++i){
        if (graph[i].second==vertex){
            vertices.push_back(graph[i].first);
        }
    }
}
