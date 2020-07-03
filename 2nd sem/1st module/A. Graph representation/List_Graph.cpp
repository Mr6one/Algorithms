#include "Header.h"
#include "List_Graph_header.h"

ListGraph::ListGraph(IGraph* InitialGraph){
    graph.resize(InitialGraph->VerticesCount());
    for (size_t i=0; i<graph.size(); ++i){
        InitialGraph->GetNextVertices(i, graph[i]);
    }
}

void ListGraph::AddEdge(int from, int to){
    graph[from].push_back(to);
}

int ListGraph::VerticesCount()const{
    return graph.size();
}

void ListGraph::GetNextVertices(int vertex, vector<int>& vertices)const{
    vertices=graph[vertex];
}

void ListGraph::GetPrevVertices(int vertex, vector<int>& vertices)const{
    for (size_t i=0; i<graph.size(); ++i){
        for (vert: graph[i]){
            if (vert==vertex){
                vertices.push_back(i);
            }
        }
    }
}
