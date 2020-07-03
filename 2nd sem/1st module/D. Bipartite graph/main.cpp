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
    int VerticesCount()const;
    void GetNextVertices(int, vector<int>&)const;
private:
    vector<vector<int>> graph;
};

void ListGraph::Add(int from, int to){
    graph[from].push_back(to);
    graph[to].push_back(from);
}

int ListGraph::VerticesCount()const{
    return graph.size();
}


void ListGraph::GetNextVertices(int vertex, vector<int>& vertices)const{
    vertices=graph[vertex];
}

bool BipartiteChecked(ListGraph& graph, int i, vector<bool>& visited, vector<int>& deapth){
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
                deapth[vert]=deapth[vertex]+1;
            } if (deapth[vert]%2==deapth[vertex]%2){
                return false;
            }
        }
    }
    return true;
}

bool BipartiteChecked(ListGraph& graph){
    vector<bool> visited(graph.VerticesCount());
    vector<int> deapth(graph.VerticesCount(), 0);
    for (int i=0; i<graph.VerticesCount(); ++i){
        if (visited[i]==false){
             if (!BipartiteChecked(graph, i, visited, deapth)){
                return false;
             }
        }
    }
    return true;
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
    if (BipartiteChecked(graph)){
        cout<<"YES";
    } else {
        cout<<"NO";
    }
    return 0;
}
