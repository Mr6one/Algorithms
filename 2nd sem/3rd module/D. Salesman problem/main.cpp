#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <math.h>
#include <unordered_map>

using namespace std;

const double maxDistance=numeric_limits<double>::max();

struct Point{
    double x;
    double y;
};

class MatrixGraph{
public:
    explicit MatrixGraph(int n): graph(n) {
        for (size_t i=0; i<graph.size(); ++i){
            graph[i].resize(n, -1);
        }
    }
    void Add(int, int, double);
    double GetFlow(int, int)const;
    int VerticesCount()const;
    void GetNextVertices(int, vector<pair<int, double>>&)const;
private:
    vector<vector<double>> graph;
};

void MatrixGraph::Add(int from, int to, double weight){
    graph[from][to]=weight;
    graph[to][from]=weight;
}

double MatrixGraph::GetFlow(int from, int to)const{
    return graph[from][to];
}

int MatrixGraph::VerticesCount()const{
    return graph.size();
}

void MatrixGraph::GetNextVertices(int vertex, vector<pair<int, double>>& vertices)const{
    for (size_t i=0; i<graph.size(); ++i){
        if (graph[vertex][i]!=-1){
            vertices.push_back(make_pair(i, graph[vertex][i]));
        }
    }
}

void GraphGeneration(int vertNum, MatrixGraph& graph){
    default_random_engine generator(time(0));
    normal_distribution<double> distribution(0.0, 1.0);
    vector<Point> vertices(vertNum);

    for (int i=0; i<vertNum; ++i){
        vertices[i].x=distribution(generator);
        vertices[i].y=distribution(generator);
    }

    for (int i=0; i<vertNum-1; ++i){
        for (int j=i+1; j<vertNum; ++j){
            double distance=sqrt(pow((vertices[i].x-vertices[j].x), 2)+pow((vertices[i].y-vertices[j].y), 2));
            graph.Add(i, j, distance);
        }
    }
}

void ComponentsCounting(const MatrixGraph& graph, int vertex, vector<bool>& visited, vector<int>& componentsVerticies, vector<int>& vertComp, int numComp){
    visited[vertex]=true;
    componentsVerticies.push_back(vertex);
    vertComp[vertex]=numComp;
    vector<pair<int, double>> nextVerticies;
    graph.GetNextVertices(vertex, nextVerticies);
    for (pair<int, double> vert: nextVerticies){
        if (!visited[vert.first]){
            ComponentsCounting(graph, vert.first, visited, componentsVerticies, vertComp, numComp);
        }
    }
}

void ComponentsCounting(const MatrixGraph& graph, vector<vector<int>>& components, vector<int>& vertComp){
    vector<bool> visited(graph.VerticesCount());
    int numComp=0;
    for (int i=0; i<graph.VerticesCount(); ++i){
        if (!visited[i]){
            vector<int> componentsVerticies;
            ComponentsCounting(graph, i, visited, componentsVerticies, vertComp, numComp);
            components.push_back(componentsVerticies);
            ++numComp;
        }
    }
}

void CountApproximatePath(const MatrixGraph& graph, int vertex, vector<bool>& visited, vector<int>& path){
    visited[vertex]=true;
    path.push_back(vertex);
    vector<pair<int, double>> nextVerticies;
    graph.GetNextVertices(vertex, nextVerticies);

    for (pair<int, double> vert: nextVerticies){
        if (!visited[vert.first]){
            CountApproximatePath(graph, vert.first, visited, path);
        }
    }
}

double CountApproximatePath(const MatrixGraph& graph, vector<int>& path){
    vector<bool> visited(graph.VerticesCount());
    CountApproximatePath(graph, 0, visited, path);
    path.push_back(0);

    double distance=0;
    for (size_t i=0; i<path.size()-1; ++i){
        distance+=graph.GetFlow(path[i], path[i+1]);
    }
    return distance;
}

void OstTreeSearch(const MatrixGraph& graph, MatrixGraph& ostTree){
    unordered_map<int, int> used;
    while (true){
        vector<vector<int>> components;
        vector<int> vertComp(graph.VerticesCount());
        ComponentsCounting(ostTree, components, vertComp);
        if (components.size()==1){
            break;
        }
        for (size_t i=0; i<components.size(); ++i){
            int firstVert;
            int secondVert;
            double minWeight=maxDistance;
            for (int vert: components[i]){
                vector<pair<int, double>> nextVerticies;
                graph.GetNextVertices(vert, nextVerticies);
                for (pair<int, double> vertex: nextVerticies){
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
}

bool PossibleWays(vector<int>& a, int n){
    int j=n-2;
    while (j!=-1 && a[j]>=a[j+1]){
        --j;
    }

    if (j==-1){
        return false;
    }

    int k=n-1;
    while (a[j] >= a[k]){
        --k;
    }

    swap(a[j], a[k]);
    int l=j+1, r=n-1;
    while (l<r){
        swap(a[l], a[r]);
        ++l;
        --r;
    }

    return true;
}

void FoundedPathOutput(const MatrixGraph& graph, vector<int>& path, int n, double& distance){
    for (size_t i=0; i<path.size()-1; ++i){
        distance+=graph.GetFlow(path[i], path[i+1]);
    }
    cout<<"founded distance:"<<' '<<distance<<endl;
    cout<<"founded path:"<<endl;
    for (int vert: path){
        cout<<vert<<' ';
    }
    cout<<endl;
}

vector<int> CountPrecisePath(const MatrixGraph& matrixGraph, int n, vector<int>& path, double& minDistance){
    vector<int> ways(n);
    for (int i=0; i<n; ++i){
        ways[i]=i;
    }
    vector<int> minPath=ways;
    minPath.push_back(0);

    while (PossibleWays(ways, n)){
        ways.push_back(ways[0]);
        double distance=0;
        for (size_t i=0; i<path.size()-1; ++i){
            distance+=matrixGraph.GetFlow(ways[i], ways[i+1]);
        }
        if (distance<minDistance){
            minDistance=distance;
            minPath=ways;
        }
        ways.pop_back();
    }
    cout<<endl;
    return minPath;
}

int main()
{
    for (int n=2; n<11; ++n){
        cout<<"vertex count:"<<' '<<n<<endl;
        int experiment_number=10;
        double average_deflection=0;
        for (int i=0; i<experiment_number; ++i){
            MatrixGraph graph(n);
            MatrixGraph ostTree(graph.VerticesCount());
            GraphGeneration(n, graph);

            OstTreeSearch(graph, ostTree);
            vector<int> path;
            double distance=CountApproximatePath(ostTree, path);

            cout<<"experiment_number:"<<' '<<i+1<<endl;
            FoundedPathOutput(graph, path, n, distance);

            double minDistance=maxDistance;
            double dist=distance;

            vector<int> minPath=CountPrecisePath(graph, n, path, minDistance);

            cout<<"min distance:"<<' '<<minDistance<<endl;
            cout<<"min path:"<<' '<<endl;
            for (int vert: minPath){
                cout<<vert<<' ';
            }
            cout<<endl;
            cout<<"deflection:"<<' '<<dist/minDistance;
            average_deflection+=dist/minDistance;
            cout<<endl<<endl;
        }
        cout<<"average deflection:"<<' '<<average_deflection/experiment_number<<endl<<endl;
    }
    return 0;
}
