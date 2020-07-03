#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class ListGraph{
public:
    ListGraph(int v){
        graph.resize(v);
    }

    void Add(int, int);
    int VerticesCount();
    void GetNextVertices(int, vector<int>&);
private:
    vector<vector<int>> graph;
};


void ListGraph::Add(int from, int to){
    graph[from].push_back(to);
}

int ListGraph::VerticesCount(){
    return graph.size();
}

void ListGraph::GetNextVertices(int vertex, vector<int>& vertices){
    vertices=graph[vertex];
}

class MatrixGraph{
public:
    MatrixGraph(int V, vector<int>& componentNumber, int componentsNumber, ListGraph& inverted_Graph){
        graph.resize(componentsNumber);
        for (size_t i=0; i<graph.size(); ++i){
            graph[i].resize(componentsNumber, 0);
        }
        for (int i=0; i<V; ++i){
            vector<int> vertices;
            inverted_Graph.GetNextVertices(i, vertices);
            for (int vertex: vertices){
                if (componentNumber[vertex]!=componentNumber[i]){
                    graph[componentNumber[i]][componentNumber[vertex]]=1;
                }
            }
        }
    }

    int VerticesCount();
    void GetNextVertices(int, vector<int>&);
    void GetPrevVertices(int, vector<int>&);
private:
    vector<vector<int>> graph;
};

int MatrixGraph::VerticesCount(){
    return graph.size();
}

void MatrixGraph::GetNextVertices(int vertex, vector<int>& vertices){
    for (size_t i=0; i<graph.size(); ++i){
        if (graph[vertex][i]==1){
            vertices.push_back(i);
        }
    }
}

void MatrixGraph::GetPrevVertices(int vertex, vector<int>& vertices){
    for (size_t i=0; i<graph.size(); ++i){
        if (graph[i][vertex]==1){
            vertices.push_back(i);
        }
    }
}

void TimeOutCalculation(ListGraph& initialGraph, int i, vector<pair<int, int>>& timeOut, vector<int>& visited, int& time){
    ++time;

    vector<int> vertices;
    initialGraph.GetNextVertices(i, vertices);

    for (int vertex: vertices){
        if (visited[vertex]==false){
            visited[vertex]=true;
            TimeOutCalculation(initialGraph, vertex, timeOut, visited, time);
            ++time;
            timeOut.push_back(make_pair(time, vertex));
        }
    }
}

void TimeOutCalculation(ListGraph& initialGraph, vector<pair<int, int>>& timeOut){
    vector<int> visited(initialGraph.VerticesCount());
    int time=-1;
    for (int i=0; i<initialGraph.VerticesCount(); ++i){
        if (visited[i]==false){
            visited[i]=true;
            TimeOutCalculation(initialGraph, i, timeOut, visited, time);
            ++time;
            timeOut.push_back(make_pair(time, i));
        }
    }
}

void ComponentsCalculation(ListGraph& invertedGraph, int i, vector<int>& componentNumber, int componentsNumber, vector<bool>& visited){
    componentNumber[i]=componentsNumber;

    vector<int> vertices;
    invertedGraph.GetNextVertices(i, vertices);

    for (int vertex: vertices){
        if (visited[vertex]==false){
            visited[vertex]=true;
            ComponentsCalculation(invertedGraph, vertex, componentNumber, componentsNumber, visited);
        }
    }
}

void ComponentsCalculation(ListGraph& invertedGraph, vector<pair<int, int>>& timeOut, vector<int>& componentNumber, int& componentsNumber){
    vector<bool> visited(invertedGraph.VerticesCount());
    for (size_t i=0; i<timeOut.size(); ++i){
        if (visited[timeOut[i].second]==false){
            visited[timeOut[i].second]=true;
            ComponentsCalculation(invertedGraph, timeOut[i].second, componentNumber, componentsNumber, visited);
            ++componentsNumber;
        }
    }
}

int MinVerticesCalculation(MatrixGraph& compressedGraph, int componentsNumber){
    int in=0;
    int out=0;
    vector<int> vertices;
    for (int i=0; i<compressedGraph.VerticesCount(); ++i){
        compressedGraph.GetNextVertices(i, vertices);
        if (vertices.size()==0){
            ++out;
        }
        vertices.clear();
        compressedGraph.GetPrevVertices(i, vertices);
        if (vertices.size()==0){
            ++in;
        }
        vertices.clear();
    }
    int result=max(in, out);
    if (componentsNumber==1){
        result=0;
    }
    return result;
}

int main()
{
    int v,e;
    cin>>v>>e;
    ListGraph initialGraph(v);
    ListGraph invertedGraph(v);
    for (int i=0; i<e; ++i){
        int from, to;
        cin>>from>>to;
        initialGraph.Add(from-1, to-1);
        invertedGraph.Add(to-1, from-1);
    }
    vector<pair<int, int>> timeOut;

    TimeOutCalculation(initialGraph, timeOut);
    sort(timeOut.begin(), timeOut.end(), greater<pair<int, int>>());

    int componentsNumber=0;
    vector<int> componentNumber(invertedGraph.VerticesCount());
    ComponentsCalculation(invertedGraph, timeOut, componentNumber, componentsNumber);

    MatrixGraph compressedGraph(v, componentNumber, componentsNumber, invertedGraph);

    cout<<MinVerticesCalculation(compressedGraph, componentsNumber);

    return 0;
}
