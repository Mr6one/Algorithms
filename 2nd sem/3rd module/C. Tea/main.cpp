//«адача сводитс€ к поиску необходимого максимального потока. ѕостроение графа: пусть изначально у нас есть двудольный граф:
//одна половина - состоит из сотрудника(кажда€ вершина - один сотрудник), друга€ - из количества различных типов ча€. —оединим
//вершины одной половинки с другой следующим образом: от каждого сотрудника проведем ребра к тем видам ча€, которые он предпочитает,
//вес кажлого ребра - количество доступных пакетиков этого типа ча€. “еперь добавим две фиктивные вершины: это будет сток и исток.
//ќт истока проведем ребро к каждому сотруднику, изначально вес каждого такого ребра - +бесконечность. ќт  аждого вида ча€ проведем ребро
//к стоку: на каждом ребре напишем количество доступных пакетиков этого типа ча€ - вес ребра. √раф построен. «апускаем поиск
//максимального потока и далее с помощью бинпоиска ищем необходимый вес ребер от истока к сотрудникам до тех пор, пока максимальный
//поток не станет равен сумме ребер от истоки к сотрудникам.

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
    void UpdateFlow(int, int, int);
    int VerticesCount()const;
    void GetNextVertices(int, vector<int>&)const;
    int GetFlow(int, int)const;
private:
    vector<vector<int>> graph;
};

void MatrixGraph::UpdateFlow(int from, int to, int weight){
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

int BinSearch(unsigned long long maxFlow, int& left, int& right, unsigned long long possibleDays, int V){
    if (maxFlow<possibleDays*V){
        right=possibleDays-1;
        return (right+left)/2;
    } else if (maxFlow>=possibleDays*V){
        left=possibleDays;
        return (right+left+1)/2;
    }
}

bool MinWayCounting(const MatrixGraph& graph, vector<int>& parents){
    queue<int> myQueue;
    vector<bool> visited(graph.VerticesCount());
    myQueue.push(graph.VerticesCount()-1);
    visited[graph.VerticesCount()-1]=true;
    while (!myQueue.empty()){
        int vertex=myQueue.front();
        myQueue.pop();

        vector<int> nextVerticies;
        graph.GetNextVertices(vertex, nextVerticies);

        for (int vert: nextVerticies){
            if (!visited[vert]){
                parents[vert]=vertex;
                if (vert==0){
                    return true;
                }
                visited[vert]=true;
                myQueue.push(vert);
            }
        }
    }
    return false;
}

unsigned long long CountingMaxFlow(MatrixGraph& graph, vector<int>& parents){
    unsigned long long maxFlow=0;
    while(MinWayCounting(graph, parents)){
        int minFlow=limit;
        for (int i=0; i!=graph.VerticesCount()-1; i=parents[i]){
            if (graph.GetFlow(parents[i], i)<minFlow){
                minFlow=graph.GetFlow(parents[i], i);
            }
        }
        for (int i=0; i!=graph.VerticesCount()-1; i=parents[i]){
            if (parents[i]==graph.VerticesCount()-1){
                maxFlow+=minFlow;
            }
            if (graph.GetFlow(parents[i], i)-minFlow>=0){
                graph.UpdateFlow(parents[i], i, -minFlow);
            }
            if (graph.GetFlow(i, parents[i])+minFlow>=0){
                graph.UpdateFlow(i, parents[i], minFlow);
            }
        }
    }
    return maxFlow;
}

int CountMaxDays(MatrixGraph& graph, int peopleNum){
    MatrixGraph usedGraph=graph;
    vector<int> parents(graph.VerticesCount());

    unsigned long long maxFlow=0;

    unsigned long long possibleDays=limit/2;
    int left=0;
    int right=limit;
    while (maxFlow!=possibleDays*peopleNum){
        graph=usedGraph;

        vector<int> nextVerticies;
        graph.GetNextVertices(graph.VerticesCount()-1, nextVerticies);
        for (int vert: nextVerticies){
            graph.UpdateFlow(graph.VerticesCount()-1, vert, possibleDays);
            graph.UpdateFlow(graph.VerticesCount()-1, vert, -1);
        }

        maxFlow = CountingMaxFlow(graph, parents);
        possibleDays=BinSearch(maxFlow, left, right, possibleDays, peopleNum);
    }
    return possibleDays;
}

int main()
{
    int peopleNum,teaTypes;
    cin>>peopleNum>>teaTypes;
    int vertexNum=peopleNum+teaTypes+2;
    MatrixGraph graph(vertexNum);
    vector<int> teaNumber(teaTypes+1);
    for (int i=0; i<teaTypes; ++i){
        int teaAmount;
        cin>>teaAmount;
        graph.UpdateFlow(i+1, 0, teaAmount);
        teaNumber[i+1]=teaAmount;
    }
    for (int i=0; i<peopleNum; ++i){
        int teaTypeNum;
        cin>>teaTypeNum;
        for (int j=0; j<teaTypeNum; ++j){
            int teaType;
            cin>>teaType;
            graph.UpdateFlow(i+teaTypes+1, teaType, teaNumber[teaType]);
        }
        graph.UpdateFlow(vertexNum-1, i+teaTypes+1, 1);
    }
    cout<<CountMaxDays(graph, peopleNum);
    return 0;
}
