#include <iostream>
#include<vector>

using namespace std;

struct Node{
    vector<int> children;
    int height;
    int number;
};

class Graph{
public:
    Graph(int n):
        nodes(n)
    {SetNodesNumbers(n);}

    void Link(int, int);
    void CountMaxDistance(vector<int>&);
private:
    vector<Node> nodes;
    void SetNodesNumbers(int);
    int GetMaxDepth(int, int);
    void MaxDepthSet(int);
    void CountMaxDistance(int, int, vector<int>&);
};

void Graph::SetNodesNumbers(int n){
    for (int i=0; i<n; ++i){
        nodes[i].number=i;
    }
}

void Graph::Link(int x, int y){
    nodes[min(x,y)].children.push_back(max(x,y));
}

void Graph::CountMaxDistance(vector<int>& MaxSummitDistance){
    MaxDepthSet(0);
    CountMaxDistance(0, 0, MaxSummitDistance);
}

int Graph::GetMaxDepth(int parent, int excluding){
    int MaxDepth=0;
    for(int child: nodes[parent].children){
        if (nodes[child].height>MaxDepth && nodes[child].number!=excluding){
            MaxDepth=nodes[child].height;
        }
    }
    return MaxDepth;
}

void Graph::MaxDepthSet(int parent){
    for(int child: nodes[parent].children){
        MaxDepthSet(child);
    }
    nodes[parent].height=GetMaxDepth(parent, -1)+1;
}

void Graph::CountMaxDistance(int parent, int CurrentMaxDistance, vector<int>& MaxSummitDistance){
    for(int child: nodes[parent].children){
        int height=GetMaxDepth(parent, nodes[child].number);
        CountMaxDistance(child, max(CurrentMaxDistance, height)+1, MaxSummitDistance);
    }
    MaxSummitDistance[nodes[parent].number]=max(GetMaxDepth(parent, -1), CurrentMaxDistance);
}

int main()
{
    int n;
    cin>>n;
    Graph graph(n);
    int x,y;
    for (int i=0; i<n-1; ++i){
        cin>>x>>y;
        graph.Link(x, y);
    }
    vector<int> MaxSummitDistance(n);
    graph.CountMaxDistance(MaxSummitDistance);
    for (int distance: MaxSummitDistance){
        cout<<distance<<endl;
    }
    return 0;
}
