#include <iostream>
#include <vector>

using namespace std;

class ListGraph{
public:
    ListGraph(int n):graph(n){
        for (size_t i=0; i<graph.size(); ++i){
            graph[i].resize(n);
            for (size_t j=0; j<graph[i].size(); ++j){
                graph[i][j]=-1;
            }
        }
    }

    void Add(int, int, double);
    int VerticesCount()const;
    double GetValue(int, int)const;
private:
    vector<vector<double>> graph;
};

void ListGraph::Add(int from, int to, double value){
        graph[from][to]=value;
}

int ListGraph::VerticesCount()const{
    return graph.size();
}

double ListGraph::GetValue(int from, int to)const{
    return graph[from][to];
}

bool SearchingArbitrage(ListGraph& graph){
    vector<vector<double>> dist(graph.VerticesCount());
    for (int vert=0; vert<graph.VerticesCount(); ++vert){
        for (size_t i=0; i<dist.size(); ++i){
            dist[i].assign(graph.VerticesCount(), 0);
        }
        dist[0][vert]=1;

        for (int k=1; k<graph.VerticesCount(); ++k){
            for (int j=0; j<graph.VerticesCount(); ++j){
                for (int i=0; i<graph.VerticesCount(); ++i){
                    if (graph.GetValue(i,j)!=-1 && graph.GetValue(i,j)*dist[k-1][i]>dist[k][j]){
                        dist[k][j]=graph.GetValue(i,j)*dist[k-1][i];
                    }
                }
            }
        }

        for(size_t i=0; i<dist.size(); ++i){
            for (size_t j=0; j<dist[i].size(); ++j){
                if (graph.GetValue(j,vert)!=-1 && dist[i][j]*graph.GetValue(j, vert)>1){
                    return true;
                }
            }
        }
    }
    return  false;
}

int main()
{
    int n;
    cin>>n;
    ListGraph graph(n);
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            if (i==j){
                continue;
            }
            double value;
            cin>>value;
            graph.Add(i, j, value);
        }
    }
    if (SearchingArbitrage(graph)){
        cout<<"YES";
    } else {
        cout<<"NO";
    }
    return 0;
}
