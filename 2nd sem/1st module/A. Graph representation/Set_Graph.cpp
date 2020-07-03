#include "Header.h"
#include "Set_Graph_header.h"
#include <iterator>

SetGraph::SetGraph(IGraph* InitialGraph){
    int size_=InitialGraph->VerticesCount();
    graph.resize(size_);
    vector<int> vertices;
    for (size_t i=0; i<graph.size(); ++i){
        InitialGraph->GetNextVertices(i, vertices);
        for (vert: vertices){
            graph[i].insert(vert);
        }
    }
}

void SetGraph::AddEdge(int from, int to){
    graph[from].insert(to);
}

int SetGraph::VerticesCount()const{
    return graph.size();
}

void SetGraph::GetNextVertices(int vertex, vector<int>& vertices)const{
    unordered_set<int>::const_iterator it=graph[vertex].begin();
    for (; it==graph[vertex].end(); ++it){
        vertices.push_back(*it);
    }
}

void SetGraph::GetPrevVertices(int vertex, vector<int>& vertices)const{
    for (size_t i=0; i<graph.size(); ++i){
        unordered_set<int>::const_iterator it=graph[i].begin();
        for (; it==graph[i].end(); ++it){
            if (*it==vertex){
                vertices.push_back(i);
            }
        }
    }
}
