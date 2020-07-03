#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

const int maxDistance=numeric_limits<int>::max();

class ListGraph{
public:
    explicit ListGraph(int n): graph(n) {}
    void Add(int, int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<pair<int, int>>&)const;
private:
    vector<vector<pair<int, int>>> graph;
};

void ListGraph::Add(int from, int to, int weight){
    graph[from].push_back(make_pair(to, weight));
    graph[to].push_back(make_pair(from, weight));
}

int ListGraph::VerticesCount()const{
    return graph.size();
}

void ListGraph::GetNextVertices(int vertex, vector<pair<int, int>>& vertices)const{
    for (pair<int, int> vert: graph[vertex]){
        vertices.push_back(vert);
    }
}

void ComponentsChecking(const ListGraph& graph, int vertex, vector<bool>& visited, vector<int>& componentsVerticies, vector<int>& vertComp, int numComp){
    visited[vertex]=true;
    componentsVerticies.push_back(vertex);
    vertComp[vertex]=numComp;
    vector<pair<int, int>> nextVerticies;
    graph.GetNextVertices(vertex, nextVerticies);
    for (pair<int, int> vert: nextVerticies){
        if (!visited[vert.first]){
            ComponentsChecking(graph, vert.first, visited, componentsVerticies, vertComp, numComp);
        }
    }
}

void ComponentsChecking(const ListGraph& graph, vector<vector<int>>& components, vector<int>& vertComp){
    vector<bool> visited(graph.VerticesCount());
    int numComp=0;
    for (int i=0; i<graph.VerticesCount(); ++i){
        if (!visited[i]){
            vector<int> componentsVerticies;
            ComponentsChecking(graph, i, visited, componentsVerticies, vertComp, numComp);
            components.push_back(componentsVerticies);
            ++numComp;
        }
    }
}

void CountMinSpanningTreeWeight(const ListGraph& graph, int vertex, vector<bool>& visited, int& MaxWeight){
    visited[vertex]=true;
    vector<pair<int, int>> nextVerticies;
    graph.GetNextVertices(vertex, nextVerticies);
    for (pair<int, int> vert: nextVerticies){
        if (!visited[vert.first]){
            MaxWeight+=vert.second;
            CountMinSpanningTreeWeight(graph, vert.first, visited, MaxWeight);
        }
    }
}

int CountMinSpanningTreeWeight(const ListGraph& graph){
    vector<bool> visited(graph.VerticesCount());
    int MaxWeight=0;
    for (int i=0; i<graph.VerticesCount(); ++i){
        if (!visited[i]){
            CountMinSpanningTreeWeight(graph, i, visited, MaxWeight);
        }
    }
    return MaxWeight;
}

ListGraph BuildSpanningTree(const ListGraph& graph){
    ListGraph ostTree(graph.VerticesCount());
    unordered_map<int, int> used;
    while (true){
    vector<vector<int>> components;
    vector<int> vertComp(graph.VerticesCount());
    ComponentsChecking(ostTree, components, vertComp);
    if (components.size()==1){
        break;
    }
    for (size_t i=0; i<components.size(); ++i){
        int firstVert;
        int secondVert;
        int minWeight=maxDistance;
        for (int vert: components[i]){
            vector<pair<int, int>> nextVerticies;
            graph.GetNextVertices(vert, nextVerticies);
            for (pair<int, int> vertex: nextVerticies){
                if (vertex.second<minWeight && vertComp[vert]!=vertComp[vertex.first]){
                    minWeight=vertex.second;
                    firstVert=vert;
                    secondVert=vertex.first;
                }
            }
        }
        if (used[firstVert]!=secondVert){
            ostTree.Add(firstVert, secondVert, minWeight);
            used[firstVert]=secondVert;
            used[secondVert]=firstVert;
        }
    }
    }
    return ostTree;
}

int main()
{
    int V,E;
    cin>>V>>E;
    ListGraph graph(V);
    for (int i=0; i<E; ++i){
        int from, to, weight;
        cin>>to>>from>>weight;
        graph.Add(to-1, from-1, weight);
    }
    ListGraph ostTree=BuildSpanningTree(graph);
    cout<<CountMinSpanningTreeWeight(ostTree);
    return 0;
}
