#include <iostream>
#include <vector>

using namespace std;

class MatrixGraph{
public:
    MatrixGraph(int n):graph(n){
        for (size_t i=0; i<graph.size(); ++i){
            graph[i].resize(n);
            for (size_t j=0; j<graph[i].size(); ++j){
                graph[i][j]=0;
            }
        }
    }

    void Add(int, int, int);
    int VerticesCount()const;
    int GetDistance(int, int)const;
private:
    vector<vector<int>> graph;
};

void MatrixGraph::Add(int from, int to, int value){
        graph[from][to]=value;
}

int MatrixGraph::VerticesCount()const{
    return graph.size();
}

int MatrixGraph::GetDistance(int from, int to)const{
    return graph[from][to];
}

void FindTheShortestDistances(MatrixGraph& graph, int n, vector<vector<int>>& theShortestDist){
    for (int i=0; i<n; ++i){
        theShortestDist[i].resize(n);
    }
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            theShortestDist[i][j]=graph.GetDistance(i, j);
        }
    }
    vector<vector<int>> parents(n);
    for (int i=0; i<n; ++i){
        parents[i].resize(n);
    }
    for (int k=0; k<n; ++k){
        for (int i=0; i<n; ++i){
            for (int j=0; j<n; ++j){
                theShortestDist[i][j]=min(theShortestDist[i][j], theShortestDist[i][k]+theShortestDist[k][j]);
                parents[i][j]=parents[i][k];
            }
        }
    }
}

int main()
{
    int n;
    cin>>n;
    MatrixGraph graph(n);
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            int distance;
            cin>>distance;
            graph.Add(i, j, distance);
        }
    }
    vector<vector<int>> theShortestDist(n);
    FindTheShortestDistances(graph, n, theShortestDist);
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            cout<<theShortestDist[i][j]<<' ';
        }
        cout<<endl;
    }
    return 0;
}
