#include <iostream>
#include <vector>
#include <set>
#include <limits>

using namespace std;

int maxDistance=numeric_limits<int>::max();

class ListGraph{
public:
    ListGraph(int n):graph(n){}

    void Add(int, int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<pair<int, int>>&)const;
private:
    vector<vector<pair<int, int>>> graph;
};

void ListGraph::Add(int from, int to, int distance){
        graph[from].push_back(make_pair(to, distance));
        graph[to].push_back(make_pair(from, distance));
}

int ListGraph::VerticesCount()const{
    return graph.size();
}

void ListGraph::GetNextVertices(int vertex, vector<pair<int, int>>& vertices)const{
    vertices=graph[vertex];
}

int DistanceFromOneToSecondPoint(ListGraph& graph, int firstPoint, int secondPoint){
    set<pair<int, int>> processingPoints;
    vector<int> dist(graph.VerticesCount(), maxDistance);
    dist[firstPoint]=0;

    processingPoints.emplace(0, firstPoint);
    while(!processingPoints.empty()){
        int vertex=processingPoints.begin()->second;
        if (vertex==secondPoint){
            break;
        }
        processingPoints.erase(processingPoints.begin());

        vector<pair<int, int>> vertices;
        graph.GetNextVertices(vertex, vertices);

        for (size_t i=0; i<vertices.size(); ++i){
            if (vertices[i].second+dist[vertex]<dist[vertices[i].first]){
                processingPoints.erase(make_pair(dist[vertices[i].first], vertices[i].first));
                dist[vertices[i].first]=vertices[i].second+dist[vertex];
                processingPoints.emplace(dist[vertices[i].first], vertices[i].first);
            }
        }
    }
    return dist[secondPoint];
}

int main()
{
    int v, e;
    cin>>v>>e;
    ListGraph graph(v);
    for (int i=0; i<e; ++i){
        int from, to, distance;
        cin>>from>>to>>distance;
        graph.Add(from, to, distance);
    }
    int firstPoint, secondPoint;
    cin>>firstPoint>>secondPoint;
    cout<<DistanceFromOneToSecondPoint(graph, firstPoint, secondPoint);
    return 0;
}
