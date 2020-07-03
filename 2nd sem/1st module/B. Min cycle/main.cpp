#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class ListGraph{
public:
    ListGraph(int N){
        graph.resize(N);
    }
    void Add(int, int);
    int VerticesCount();
    void GetNextVertices(int, vector<int>&);
private:
    vector<vector<int>> graph;
};

void ListGraph::Add(int from, int to){
    graph[from].push_back(to);
    graph[to].push_back(from);
}

int ListGraph::VerticesCount(){
    return graph.size();
}

void ListGraph::GetNextVertices(int vertex, vector<int>& vertices){
    vertices=graph[vertex];
}

int MinCycleSearch(ListGraph& graph, int i, int maxCycle){
    vector<bool> visited(graph.VerticesCount());
    vector<int> deapth(graph.VerticesCount(), 0);
    vector<int> parents(graph.VerticesCount());

    queue<int> queueForBFS;
    queueForBFS.push(i);
    visited[i]=true;
    while (!queueForBFS.empty()){
        int vertex=queueForBFS.front();
        queueForBFS.pop();

        vector<int> vertices;
        graph.GetNextVertices(vertex, vertices);

        for (int vert: vertices){
            if (visited[vert]==false){
                queueForBFS.push(vert);
                visited[vert]=true;
                parents[vert]=vertex;
                deapth[vert]=deapth[vertex]+1;
            } else if (vert!=parents[vertex]){
                return deapth[vertex]+deapth[vert]+1;
            }
        }
    }
    return maxCycle;
}

int CycleSearch(ListGraph& graph, int minCycle, int maxCycle){
    for (int i=0; i<graph.VerticesCount(); ++i){
        int cycle=MinCycleSearch(graph, i, maxCycle);
        if (cycle<minCycle){
            minCycle=cycle;
        }
    }
    return minCycle;
}

int main()
{
    int n, m;
    cin>>n>>m;
    ListGraph graph(n);
    for (int i=0; i<m; ++i){
        int from, to;
        cin>>from>>to;
        graph.Add(from, to);
    }
    int minCycle=CycleSearch(graph, m+1, m+1);
    if (minCycle==m+1){
        cout<<-1;
    } else {
        cout<<minCycle;
    }
    return 0;
}
