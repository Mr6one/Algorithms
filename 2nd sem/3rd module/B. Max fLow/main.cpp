#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int limit=numeric_limits<int>::max();

class MatrixGraph{
public:
    explicit MatrixGraph(int n): graph(n) {
        for (size_t i=0; i<graph.size(); ++i){
            graph[i].resize(n, 0);
        }
    }
    void Update(int, int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<int>&)const;
    int GetFlow(int, int)const;
private:
    vector<vector<int>> graph;
};

void MatrixGraph::Update(int from, int to, int weight){
    graph[from][to]+=weight;
}

int MatrixGraph::VerticesCount()const{
    return graph.size();
}

void MatrixGraph::GetNextVertices(int vertex, vector<int>& vertices)const{
    for (size_t i=0; i<graph[vertex].size(); ++i){
        if (graph[vertex][i]!=0){
            vertices.push_back(i);
        }
    }
}

int MatrixGraph::GetFlow(int from, int to)const{
    return graph[from][to];
}

bool CountingMinWay(MatrixGraph& graph, vector<int>& parents){
    queue<int> myQueue;
    vector<bool> visited(graph.VerticesCount());
    myQueue.push(0);
    visited[0]=true;
    while (!myQueue.empty()){
        int vertex=myQueue.front();
        myQueue.pop();

        vector<int> nextVerticies;
        graph.GetNextVertices(vertex, nextVerticies);

        for (int vert: nextVerticies){
            if (!visited[vert]){
                parents[vert]=vertex;
                if (vert==graph.VerticesCount()-1){
                    return true;
                }
                visited[vert]=true;
                myQueue.push(vert);
            }
        }
    }
    return false;
}

int MaxFlow(const MatrixGraph& graph){
    vector<int> parents(graph.VerticesCount());
    MatrixGraph theResidualNetwork=graph;
    int maxFlow=0;
    while(CountingMinWay(theResidualNetwork, parents)){
        int minFlow=limit;
        for (int i=theResidualNetwork.VerticesCount()-1; i!=0; i=parents[i]){
            if (theResidualNetwork.GetFlow(parents[i], i)<minFlow){
                minFlow=theResidualNetwork.GetFlow(parents[i], i);
            }
        }
        for (int i=theResidualNetwork.VerticesCount()-1; i!=0; i=parents[i]){
            if (parents[i]==0){
                maxFlow+=minFlow;
            }
            if (theResidualNetwork.GetFlow(parents[i], i)-minFlow>=0){
                theResidualNetwork.Update(parents[i], i, -minFlow);
            }
            theResidualNetwork.Update(i, parents[i], minFlow);
        }
    }
    return maxFlow;
}

int main()
{
    int V,E;
    cin>>V>>E;
    MatrixGraph graph(V);
    for (int i=0; i<E; ++i){
        int from, to, weight;
        cin>>to>>from>>weight;
        graph.Update(to-1, from-1, weight);
    }
    cout<<MaxFlow(graph);
    return 0;
}
