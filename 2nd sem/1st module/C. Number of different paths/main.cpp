#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class ListGraph{
public:
    ListGraph(int n){
        graph.resize(n);
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

int CountingPass(ListGraph& graph, int first, int second){
    vector<int> countWays(graph.VerticesCount(), 0);
    vector<bool> visited(graph.VerticesCount());
    vector<int> deapth(graph.VerticesCount(), 0);

    queue<int> queueForBFS;
    queueForBFS.push(first);
    visited[first]=true;
    deapth[second]=graph.VerticesCount();
    countWays[first]=1;
    while (!queueForBFS.empty()){
        int vertex=queueForBFS.front();
        queueForBFS.pop();

        vector<int> vertices;
        graph.GetNextVertices(vertex, vertices);

        for (int vert: vertices){
            if (visited[vert]==false && deapth[vert]<=deapth[second]){
                queueForBFS.push(vert);
                visited[vert]=true;
                countWays[vert]=countWays[vertex];
                deapth[vert]=deapth[vertex]+1;
            } else if (deapth[vert]==deapth[vertex]+1){
                countWays[vert]+=countWays[vertex];
            }
        }
    }
    return countWays[second];
}

int main()
{
    int n,m;
    cin>>n>>m;
    ListGraph graph(n);
    for (int i=0; i<m; ++i){
        int from, to;
        cin>>from>>to;
        graph.Add(from, to);
    }
    int first, second;
    cin>>first>>second;
    cout<<CountingPass(graph, first, second);
    return 0;
}
